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
#include <catalyst/memory/allocator.h>
#include <catalyst/utilities/pointer_math.h>
#include <malloc.h>

typedef struct {
    uint8_t     adjustment;
} allocation_header_t;

typedef struct allocator_internal_t {
    void*       start;
    uintptr_t   allocation_top;
} allocator_internal_t;

void*   stack_allocator_allocate(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment) {
    if(allocation_size == 0) {
        CATALYST_ERROR("Allocation of size %zu bytes is forbidden. Issued to allocator %p", allocation_size, allocator);
        return NULL;
    }
    void*   current_position    =   pointer_add(allocator->internal->start, allocator->size); 
    uint8_t adjustment          =   ALIGN_FORWARD_ADJUSTMENT_WITH_HEADER(current_position, alignment, allocation_header_t);

    if(allocator->size + adjustment + allocation_size > allocator->capacity) {
        CATALYST_ERROR("Allocator %p is incapable of allocating %zu bytes", allocator, allocation_size);
        return NULL;
    }
    uintptr_t aligned_address   =   (uintptr_t)current_position + adjustment;

    allocation_header_t* header =   (allocation_header_t *)(aligned_address - sizeof(allocation_header_t));
    header->adjustment          =   adjustment;
    
    allocator->size             +=  allocation_size + adjustment;
    
    allocator->internal->allocation_top =   aligned_address;
    CATALYST_TRACE(     "Allocator %p of type CATALYST_ALLOCATOR_TYPE_STACK allocated %zu bytes successfully",
                        allocator, allocation_size
                  );
    return (void *)aligned_address;
}

void    stack_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    if((uintptr_t)pointer != allocator->internal->allocation_top) {
        CATALYST_ERROR(
                            "The provided pointer %p to the allocator %p of type CATALYST_ALLOCATOR_TYPE_STACK is not the"
                            "last allocation",
                            pointer, allocator
                            );
        return ;
    }
    void*   current_position    =   pointer_add(allocator->internal->start, allocator->size);

    allocation_header_t* header =   (allocation_header_t *)pointer_sub(pointer, sizeof(allocation_header_t));
    allocator->size             -=  (uintptr_t)current_position - (uintptr_t)pointer + header->adjustment;
    
    CATALYST_TRACE(
                    "The provided pointer %p to the allocator %p of type CATALYST_ALLOCATOR_TYPE_STACK is freed",
                    pointer, allocator
                  );
}

void    stack_allocator_reset(catalyst_allocator_t* allocator) {
    allocator->size                         =   0;
    allocator->internal->allocation_top     =   (uintptr_t)NULL;
}

void    destruct_stack_allocator(catalyst_allocator_t* allocator) {
    catalyst_deallocate(allocator->internal);
    allocator->size     =   0;
    allocator->capacity =   0;
    allocator->start    =   NULL;
    
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_STACK as been successfully destructed", allocator);
}

void    internal_construct_stack_allocator(catalyst_allocator_t* allocator) {
    allocator->allocate                 =   stack_allocator_allocate;
    allocator->deallocate               =   stack_allocator_deallocate;
    allocator->reset                    =   stack_allocator_reset;
    allocator->destruct                 =   destruct_stack_allocator;
    void*   start                       =   allocator->start;
    allocator->internal                 =   (allocator_internal_t *)CATALYST_ALLOCATE(allocator_internal_t);
    allocator->internal->start          =   start;
    allocator->internal->allocation_top =   (uintptr_t)NULL;    
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_STACK as been successfully constructed", allocator);
}
