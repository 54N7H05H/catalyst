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
#ifndef CATALYST_INCLUDE__INTERNAL__WINDOW_H_
#define CATALYST_INCLUDE__INTERNAL__WINDOW_H_
#include <catalyst/window.h>

typedef void (* catalyst_window_get_dimension_t)(const catalyst_window_t* window, int width, int height);
typedef void (* catalyst_window_get_position_t)(const catalyst_window_t* window, int* x, int* y);
typedef void (* catalyst_window_set_position_t)(const catalyst_window_t* window, int x, int y); 
typedef void (* catalyst_window_poll_events_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_wait_events_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_minimize_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_maximize_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_restore_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_show_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_hide_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_focus_t)(const catalyst_window_t* window);
typedef void (* catalyst_window_request_attention_t)(const catalyst_window_t* window);

typedef bool (* catalyst_window_window_should_close_t)(const catalyst_window_t* window);
typedef struct catalyst_native_window_t catalyst_native_window_t;
typedef struct catalyst_window_t {
    catalyst_window_get_position_t          get_position;
    catalyst_window_set_position_t          set_position;
    catalyst_window_poll_events_t           poll_events;
    catalyst_window_wait_events_t           wait_events;
    catalyst_window_minimize_t              minimize;
    catalyst_window_maximize_t              maximize;
    catalyst_window_restore_t               restore;
    catalyst_window_show_t                  show;
    catalyst_window_hide_t                  hide;
    catalyst_window_focus_t                 focus;
    catalyst_window_request_attention_t     request_attention;
    catalyst_window_window_should_close_t   should_close;

    catalyst_window_attribute_t             attributes;
    char                                    key_state[32];
    char                                    mouse_button_state[1];
    bool                                    close_requested;

    catalyst_native_window_t*               native_window;
} catalyst_window_t;

void    catalyst_construct_window(catalyst_window_t* window, int width, int height,
                                    const char* title, catalyst_window_attribute_t attributes);
void    catalyst_destruct_window(catalyst_window_t* window);

#endif