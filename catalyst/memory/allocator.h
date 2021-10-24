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
#ifndef CATALYST_ALLOCATOR__MEMORY_ALLOCATOR_H_
#define CATALYST_ALLOCATOR__MEMORY_ALLOCATOR_H_

#include <catalyst/memory.h>

typedef void*   (* allocator_allocate_t)(catalyst_allocator_t* allocator, size_t allocation_size, uint8_t alignment);
typedef void    (* allocator_deallocate_t)(catalyst_allocator_t* allocator, void* pointer);
typedef void    (* allocator_reset_t)(catalyst_allocator_t* allocator);
typedef void    (* allocator_destruct_t)(catalyst_allocator_t* allocator);

typedef struct allocator_internal_t allocator_internal_t;
typedef struct catalyst_allocator_t {
    allocator_allocate_t        allocate;
    allocator_deallocate_t      deallocate;
    allocator_reset_t           reset;
    allocator_destruct_t        destruct;
    union {
        allocator_internal_t*       internal;
        void*                       start;
    };
    size_t                      size;
    size_t                      capacity;
} catalyst_allocator_t;

void internal_construct_system_allocator(catalyst_allocator_t* allocator);
void internal_construct_linear_allocator(catalyst_allocator_t* allocator);
void internal_construct_stack_allocator(catalyst_allocator_t* allocator);
void internal_construct_pool_allocator(catalyst_allocator_t* allocator, size_t object_size);
void internal_construct_free_list_allocator(catalyst_allocator_t* allocator);

#endif
