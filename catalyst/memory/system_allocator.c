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
#include <limits.h>
#include <malloc.h>

void*   system_allocator_allocate(catalyst_allocator_t* allocator, size_t size, uint8_t alignment) {
    if(size == 0) {
        CATALYST_ERROR("Allocation of 0 bytes is not allowed. Allocation requested from allocator %p", allocator);
        return NULL;
    }
    return malloc(size);
}

void    system_allocator_deallocate(catalyst_allocator_t* allocator, void* pointer) {
    CATALYST_WARN("System allocators are incapable of knowing whether the pointer passed is allocated by them");
    free(pointer);
}

void    system_allocator_reset(catalyst_allocator_t* allocator) {
    CATALYST_WARN("System allocators are incapable of resetting. Issued to allocator %p", allocator);
}

void    destruct_system_allocator(catalyst_allocator_t* allocator) {
    CATALYST_WARN("System allocators are incapable of destrucing. Issued to allocator %p", allocator);
}

void    internal_construct_system_allocator(catalyst_allocator_t* allocator) {
    allocator->allocate     =   system_allocator_allocate;
    allocator->deallocate   =   system_allocator_deallocate;
    allocator->reset        =   system_allocator_reset;
    allocator->destruct     =   destruct_system_allocator;
    
    allocator->size         =   0;
    allocator->capacity     =   UINT_MAX; 
    CATALYST_TRACE("Allocator %p of type CATALYST_ALLOCATOR_TYPE_SYSTEM has been successfully constructed", allocator);
}
