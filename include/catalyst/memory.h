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
#ifndef CATALYST_INCLUDE__MEMORY_H_
#define CATALYST_INCLUDE__MEMORY_H_

#include <catalyst/compiler.h>

void catalyst_initialize_memory_subsystem();
void catalyst_terminate_memory_subsystem();

typedef enum {
    CATALYST_ALLOCATOR_TYPE_SYSTEM,
    CATALYST_ALLOCATOR_TYPE_FREELIST,
    CATALYST_ALLOCATOR_TYPE_POOL,
    CATALYST_ALLOCATOR_TYPE_STACK,
    CATALYST_ALLOCATOR_TYPE_LINEAR,
} catalyst_allocator_type_t;

typedef struct catalyst_allocator_t catalyst_allocator_t;

catalyst_allocator_t*   catalyst_create_allocator(
            catalyst_allocator_type_t allocator_type, void* memory, size_t size, size_t object_size
        );
void                    catalyst_delete_allocator(catalyst_allocator_t* allocator);

void*   catalyst_allocator_allocate(catalyst_allocator_t* allocator, size_t size, uint8_t alignment);
void    catalyst_allocator_deallocate(catalyst_allocator_t* allocator, void* memory);
void    catalyst_allocator_reset(catalyst_allocator_t* allocator);
size_t  catalyst_allocator_capacity(catalyst_allocator_t* allocator);
size_t  catalyst_allocator_free_size(catalyst_allocator_t* allocator);

void    catalyst_allocator_set_default(catalyst_allocator_t* allocator);
catalyst_allocator_t* catalyst_allocator_get_default();

void*   catalyst_allocate(size_t size, uint8_t alignment);
void    catalyst_deallocate(void* pointer);

#define CATALYST_ALLOCATE(type)             catalyst_allocate(sizeof (type), alignof (type))
#define CATALYST_ALLOCATE_ARRAY(type, size) catalyst_allocate(sizeof (type) * size, alignof (type))
#define CATALYST_DEALLOCATE(pointer)        catalyst_deallocate(pointer)

#endif
