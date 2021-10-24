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

#ifndef CATALYST_INCLUDE__UTILITIES__POINTER_MATH_H_
#define CATALYST_INCLUDE__UTILITIES__POINTER_MATH_H_

#include <catalyst/compiler.h>

#define pointer_add(pointer, offset)    (void *)(((uintptr_t)pointer) + offset)
#define pointer_sub(pointer, offset)    (void *)(((uintptr_t)pointer) - offset)

#define ALIGN_FORWARD_ADJUSTMENT_WITH_HEADER(address, alignment, header_type) \
    align_forward_adjustment_with_header(address, alignment, sizeof (header_type), alignof(header_type))

static inline void*     align_forward(const void* address, uint8_t alignment) {
    return (void *)(((uintptr_t)address + (uintptr_t)(alignment - 1)) & (uintptr_t)(~(alignment - 1)));
}

static inline void*     align_backward(const void* address, uint8_t alignment) {
    return (void *)((uintptr_t)address & (uintptr_t)(~(alignment - 1)));
}

static inline uint8_t   align_forward_adjustment(const void* address, uint8_t alignment) {
    uint8_t adjustment = alignment - ((uintptr_t)address & (uintptr_t)(alignment - 1));
    if(adjustment == alignment)
        return 0;
    return adjustment;
}

static inline uint8_t   align_backward_adjustment(const void* address, uint8_t alignment) {
    uint8_t adjustment = ((uintptr_t)address & (uintptr_t)(alignment - 1));
    if(adjustment == alignment) return 0;
    return adjustment;
}

static inline uint8_t   align_forward_adjustment_with_header(
                                                        const void* address, uint8_t alignment,
                                                        size_t header_size, uint8_t header_alignment
                                                    ) {
    if(header_alignment > alignment)
        alignment = header_alignment;

    return (uint8_t)(header_size + align_forward_adjustment(pointer_add(address, header_size), alignment));
}

static inline bool      is_aligned(const void* address, uint8_t alignment) {
    return align_forward_adjustment(address, alignment) == 0;
}

#endif
