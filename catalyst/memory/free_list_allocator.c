/*
**
**   ▄████▄   ▄▄▄     ▄▄▄█████▓ ▄▄▄       ██▓   ▓██   ██▓  ██████ ▄▄▄█████▓
**  ▒██▀ ▀█  ▒████▄   ▓  ██▒ ▓▒▒████▄    ▓██▒    ▒██  ██▒▒██    ▒ ▓  ██▒ ▓▒
**  ▒▓█    ▄ ▒██  ▀█▄ ▒ ▓██░ ▒░▒██  ▀█▄  ▒██░     ▒██ ██░░ ▓██▄   ▒ ▓██░ ▒░
**  ▒▓▓▄ ▄██▒░██▄▄▄▄██░ ▓██▓ ░ ░██▄▄▄▄██ ▒██░     ░ ▐██▓░  ▒   ██▒░ ▓██▓ ░
**  ▒ ▓███▀ ░ ▓█   ▓██▒ ▒██▒ ░  ▓█   ▓██▒░██████▒ ░ ██▒▓░▒██████▒▒  ▒██▒ ░
**  ░ ░▒ ▒  ░ ▒▒   ▓▒█░ ▒ ░░    ▒▒   ▓▒█░░ ▒░▓  ░  ██▒▒▒ ▒ ▒▓▒ ▒ ░  ▒ ░░
**    ░  ▒     ▒   ▒▒ ░   ░      ▒   ▒▒ ░░ ░ ▒  ░▓██ ░▒░ ░ ░▒  ░ ░    ░
**    ░          ░   ▒    ░        ░   ▒     ░ ░   ▒ ▒ ░░  ░  ░  ░    ░
**    ░ ░            ░  ░              ░  ░    ░  ░░ ░           ░
**    ░                                            ░ ░
**
**  Copyright (c) 2021 - present, Santhosh K (0xCAFECODE)
**
**  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
**  files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
**  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
**  Software is furnished to do so, subject to the following conditions:
**
**  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
**  Software.
**
**  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
**  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
**  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
**  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
*/
#include <catalyst/logging.h>
#include <catalyst/memory.h>
#include <catalyst/memory/allocator.h>
#include <catalyst/utilities/pointer_math.h>

typedef struct {
    size_t  allocation_size;
    uint8_t allocation_adjustment;
} allocation_header_t;

typedef struct free_block_t {
    size_t                  size;
    struct free_block_t*    next;
} free_block_t;

typedef struct allocator_internal_t {
    void*           start;
    free_block_t*   free_blocks;
} allocator_internal_t;

void*   free_list_allocator_allocate(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment) {
    if(allocation_size == 0) {
        CATALYST_ERROR("Allocation of size 0 bytes is prohibited. Issued on allocator %p", allocator);
        return NULL;
    } else if(alignment == 0) {
        CATALYST_ERROR("Allocation alignment of 0 bytes is prohibited. Issued on allocator %p", allocator);
        return NULL;
    }
    allocator_internal_t* internal      =   allocator->internal;

    free_block_t*   previous_free_block =   NULL;
    free_block_t*   current_free_block  =   allocator->internal->free_blocks;
    free_block_t*   best_fit_previous   =   NULL;
    free_block_t*   best_fit_current    =   NULL;
    uint8_t         best_fit_adjustment =   0;
    size_t          best_fit_capacity   =   0;

    while(current_free_block != NULL) {
        uint8_t adjustment  =   ALIGN_FORWARD_ADJUSTMENT_WITH_HEADER(internal->free_blocks, alignment, allocation_header_t);
        size_t  capacity    =   allocation_size + adjustment;

        if(current_free_block->size = capacity) {
            //  A free block of the required size is present
            best_fit_previous   =   previous_free_block;
            best_fit_current    =   current_free_block;
            best_fit_adjustment =   adjustment;
            best_fit_capacity   =   capacity;

            break;
        }

        if( current_free_block->size > capacity
            && (best_fit_current == NULL || current_free_block->size < best_fit_current->size)
          ) {
            
        }

        if(
            (current_free_block->size == capacity) || 
            (current_free_block->size > capacity &&
                (best_fit_current == NULL || current_free_block->size < best_fit_current->size))        
        ) {
            best_fit_previous   =   previous_free_block;
            best_fit_current    =   current_free_block;
            best_fit_adjustment =   adjustment;
            best_fit_capacity   =   capacity;
        }

        if(current_free_block->size == capacity)
            break;

        previous_free_block     =   current_free_block;
        current_free_block      =   current_free_block->next;
    }

    if(best_fit_current == NULL) {
        CATALYST_ERROR(
                        "No free blocks where found for allocation of size %zu and alignment %hu. Issued to allocator %p",
                        allocation_size, alignment, allocator
                      );
        return NULL;
    }

    if(best_fit_current->size - best_fit_capacity <= sizeof (allocation_header_t)) {
        best_fit_capacity   =   best_fit_current->size;
        if(best_fit_previous == NULL)
            internal->free_blocks   =   best_fit_current->next;
        else
            best_fit_previous->next =   best_fit_current->next;
    } else {
        if(best_fit_current->size <= sizeof (free_block_t)) {
            CATALYST_ERROR(
                            "Best fit size was less than or equal to the FreeBlock Header size. Issued to allocator %p",
                            allocator
                          );
            return NULL;
        }

        free_block_t*   new_block   =   (free_block_t *)pointer_add(best_fit_current, best_fit_capacity);
        new_block->size             =   best_fit_current->size - best_fit_capacity;
        new_block->next             =   best_fit_current->next;

        if(best_fit_previous == NULL)
            internal->free_blocks   =   new_block;
        else
            best_fit_previous->next =   new_block;
    }

    uintptr_t aligned_address       =   (uintptr_t)best_fit_current + best_fit_adjustment;
    allocation_header_t* header     =   (allocation_header_t *)(aligned_address - sizeof (allocation_header_t));
    header->allocation_size         =   best_fit_capacity;
    header->allocation_adjustment   =   best_fit_adjustment;

    allocator->size +=  best_fit_capacity;

    CATALYST_TRACE("Allocation of size %zu has been made by the allocator %p successfully", allocation_size, allocator);
    return (void *)aligned_address;
}

void    free_list_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    //  TODO
    //  TODO
    //  TODO
}

void    free_list_allocator_reset(catalyst_allocator_t* allocator) {
    allocator->internal->free_blocks        =   (free_block_t *)allocator->internal->start;
    allocator->internal->free_blocks->size  =   allocator->capacity;
    allocator->internal->free_blocks->next  =   NULL;

    allocator->size =   0;
}

void    destruct_free_list_allocator(catalyst_allocator_t* allocator) {
    allocator->reset(allocator);
    CATALYST_DEALLOCATE(allocator->internal);

    allocator->start    =   NULL;
    allocator->size     =   0;
    allocator->capacity =   0;
}

void internal_construct_free_list_allocator(catalyst_allocator_t* allocator) {
    if(allocator->size <= sizeof (free_block_t)) {
        CATALYST_ERROR(
                        "The alloted memory size for the allocator %p of type CATALYST_ALLOCATOR_TYPE_FREE_LIST"
                        "should be greater than %zu", allocator, sizeof (free_block_t));
    }

    allocator->allocate         =   free_list_allocator_allocate;
    allocator->deallocate       =   free_list_allocator_deallocate;
    allocator->reset            =   free_list_allocator_reset;
    allocator->destruct         =   destruct_free_list_allocator;

    void* start                 =   allocator->start;
    allocator->internal         =   CATALYST_ALLOCATE(allocator_internal_t);
    allocator->internal->start  =   start;
    allocator->reset(allocator);
}
