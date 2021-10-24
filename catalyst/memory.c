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
#include <catalyst/utilities/pointer_math.h>
#include <catalyst/memory/allocator.h>
#include <malloc.h>

static catalyst_allocator_t* default_allocator = NULL;
static bool is_default_constructed_allocator = false;
static catalyst_allocator_t system_allocator;

void catalyst_initialize_memory_subsystem() {
    internal_construct_system_allocator(&system_allocator);
    default_allocator   = &system_allocator;
    is_default_constructed_allocator = true;
}

void catalyst_terminate_memory_subsystem() {
    if(!is_default_constructed_allocator)
        catalyst_delete_allocator(&system_allocator);
}

catalyst_allocator_t* catalyst_create_allocator(
            catalyst_allocator_type_t allocator_type, void* memory, size_t size, size_t object_size
        ) {
    catalyst_allocator_t* new_allocator = (catalyst_allocator_t *)malloc(sizeof (catalyst_allocator_t));
    
    new_allocator->size     =   0;
    new_allocator->capacity =   size;
    new_allocator->start    =   memory;

    switch(allocator_type) {
        case CATALYST_ALLOCATOR_TYPE_SYSTEM:    internal_construct_system_allocator(new_allocator);
                                                break;
        case CATALYST_ALLOCATOR_TYPE_LINEAR:    internal_construct_linear_allocator(new_allocator);
                                                break;
        case CATALYST_ALLOCATOR_TYPE_STACK:     internal_construct_stack_allocator(new_allocator);
                                                break;
        case CATALYST_ALLOCATOR_TYPE_POOL:      internal_construct_pool_allocator(new_allocator, object_size);
                                                break;
        case CATALYST_ALLOCATOR_TYPE_FREELIST:  internal_construct_free_list_allocator(new_allocator);
                                                break;
        default:                                CATALYST_ERROR("Invalid allocator type is provided %zu", allocator_type);
    }

    return new_allocator;
}

void catalyst_delete_allocator(catalyst_allocator_t* allocator) {
    if(allocator == &system_allocator)
        CATALYST_ERROR("Cannot delete system allocator %p", system_allocator);
    else
        allocator->destruct(allocator);
    free(allocator);
}

void*   catalyst_allocator_allocate(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment) {
    if(allocation_size == 0) {
        CATALYST_ERROR("Cannort make allocation of size 0");
        return NULL;
    }

    if(allocation_size + allocator->size > allocator->capacity) {
        CATALYST_ERROR("Allocator %p is incapable of allocating %zu bytes", allocation_size);
        return NULL;
    }
    void* allocation = allocator->allocate(allocator, allocation_size, alignment);
    return allocation;
}

void    catalyst_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    if(allocator == NULL) {
        CATALYST_ERROR("The provided allocator is NULL");
        return;
    }
    if(allocator == &system_allocator || pointer >= allocator->start && pointer <= pointer_add(allocator->start, allocator->capacity)) {
        allocator->deallocate(allocator, pointer);   
    } else {
        CATALYST_ERROR("The provided memory %p is not allocated by the allocator {%p}", pointer, allocator);
    }
}

void    catalyst_allocator_reset(catalyst_allocator_t* allocator) {
    if(allocator == NULL) {
        CATALYST_ERROR("The provided allocator is NULL");
        return ;
    }
    allocator->reset(allocator);
}

size_t  catalyst_allocator_capacity(catalyst_allocator_t* allocator) {
    return allocator->capacity;
}

size_t  catalyst_allocator_free_size(catalyst_allocator_t* allocator) {
    return allocator->capacity - allocator->size;
}

void    catalyst_allocator_set_default(catalyst_allocator_t* allocator) {
    if(allocator != &system_allocator)
        is_default_constructed_allocator = false;
    default_allocator = allocator;
}

catalyst_allocator_t*   catalyst_allocator_get_default() {
    return default_allocator;
}

void*   catalyst_allocate(size_t allocation_size, uint8_t alignment) {
    return catalyst_allocator_allocate(&system_allocator, allocation_size, alignment);
}

void    catalyst_deallocate(void* pointer) {
    catalyst_allocator_deallocate(&system_allocator, pointer);
}
