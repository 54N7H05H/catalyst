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

typedef struct allocator_internal_t {
    void*   start;
    void**  free_list;
    size_t  object_size;
    uint8_t object_alignment;
} allocator_internal_t;

void*   pool_allocator_allocate(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment) {
    if(allocation_size == 0) {
        CATALYST_ERROR("Allocation of size 0 bytes is prohibited. Issued to allocator %p", allocator);
        return NULL;
    }

    if(allocation_size != allocator->internal->object_size) {
        CATALYST_ERROR(
                        "Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL is defined to hold objects of size %zu",
                        allocator, allocator->internal->object_size
                      );
        return NULL;
    }

    if(alignment != allocator->internal->object_alignment) {
        CATALYST_ERROR(
                        "Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL is defined to hold objects of alignment %hu",
                        allocator, allocator->internal->object_alignment
                      );
        return NULL;
    }

    if(allocator->internal->free_list == NULL) {
        CATALYST_WARN("Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL is full", allocator);
        return NULL;
    }
    void* pointer   =   allocator->internal->free_list;
    allocator->internal->free_list  =   (void **)(*(allocator->internal->free_list));
    allocator->size +=  allocation_size;

    CATALYST_TRACE(
                        "Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL successfully allocated %zu bytes",
                        allocator, allocation_size
                  );
    return pointer;
}

void    pool_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    *((void **)pointer) = allocator->internal->free_list;
    allocator->internal->free_list  =   (void **)pointer;
    allocator->size                 -=  allocator->internal->object_size;
}

void    pool_allocator_reset(catalyst_allocator_t* allocator) {
    void** pointer = allocator->internal->free_list;
    size_t number_of_objects = allocator->capacity / allocator->internal->object_size;
    for(size_t i = 0; i < number_of_objects; ++i) {
        *pointer =  pointer_add(pointer, allocator->internal->object_size);
        pointer = (void **)*pointer;
    }
    *pointer = NULL;
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL has been reset successfully.", allocator);
}

void    destruct_pool_allocator(catalyst_allocator_t* allocator) {
    allocator->internal->free_list = NULL;
    catalyst_deallocate(allocator->internal);

    allocator->start    =   NULL;
    allocator->size     =   0;
    allocator->capacity =   0;
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL has been destructed successfully.", allocator);
}

void    internal_construct_pool_allocator(catalyst_allocator_t* allocator, size_t object_size) {
    allocator->allocate     =   pool_allocator_allocate;
    allocator->deallocate   =   pool_allocator_deallocate;
    allocator->reset        =   pool_allocator_reset;
    allocator->destruct     =   destruct_pool_allocator;
    void* start             =   allocator->start;

    allocator->internal     =   CATALYST_ALLOCATE(allocator_internal_t);
    uint8_t adjustment      =   align_forward_adjustment(start, allocator->internal->object_alignment);
    allocator->internal->free_list  =   (void **)pointer_add(start, adjustment);
    
    allocator->reset(allocator);
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_POOL has been constructed successfully.", allocator);
}
