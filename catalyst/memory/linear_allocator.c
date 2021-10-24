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

void*   linear_allocator_allocate(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment) {
    if(allocation_size == 0) {
        CATALYST_ERROR("Allocation size should not be 0. Issued to allocator {%p}", allocator);
        return NULL;
    }
    
    void*   current_position    =   pointer_add(allocator->start, allocator->size);
    uint8_t adjustment          =   align_forward_adjustment(pointer_add(allocator->start, allocator->size), alignment);
    
    if(allocator->size + allocation_size + adjustment > allocator->capacity) {
        CATALYST_ERROR(
                        "Allocation of size %zu and alignment %hu cannot be made on allocator %p",
                        allocation_size, alignment, allocator
                      );
        return NULL;
    }

    uintptr_t aligned_address   =   (uintptr_t)current_position + adjustment;
    current_position            =   pointer_add(aligned_address, allocation_size);
    allocator->size             =   (uintptr_t)current_position - (uintptr_t)allocator->start;

    CATALYST_TRACE(
                        "Allocator %p of type CATALYST_ALLOCATOR_TYPE_LINEAR made allocation of size %zu at %p",
                        allocator, allocation_size, (void *)aligned_address
                  );
    return (void *)aligned_address;
}

void    linear_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    CATALYST_WARN("Allocator %p is a linear allocator. Deallocations are not supported in a linear allocator");
}

void    linear_allocator_reset(catalyst_allocator_t* allocator) {
    allocator->size = 0;
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_LINEAR has been successfully reset");
}

void    destruct_linear_allocator(catalyst_allocator_t* allocator) {
    allocator->size = 0;
    allocator->capacity = 0;
    allocator->start = NULL;

    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_LINEAR has been successfully destructed");
}

void    internal_construct_linear_allocator(catalyst_allocator_t* allocator) {
    allocator->allocate     =   linear_allocator_allocate;
    allocator->deallocate   =   linear_allocator_deallocate;
    allocator->reset        =   linear_allocator_reset;
    allocator->destruct     =   destruct_linear_allocator;

    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_LINEAR has been successfully constructed");
}
