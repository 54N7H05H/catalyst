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
#include <catalyst/memory.h>
#include <catalyst/window.h>
#include <catalyst/internal/window.h>

void                reset_window_events(catalyst_window_t* window);
catalyst_window_t*  catalyst_create_window(int window_x, int winndow_y,
                                            int width, int height,
                                            const char* window_title,
                                            catalyst_window_attribute_t attributes) {

    catalyst_window_t* new_window = CATALYST_ALLOCATE(catalyst_window_t);
    reset_window_events(new_window);
    catalyst_construct_window(new_window, width, height, window_title, attributes);
    return new_window;
    
}


void                catalyst_delete_window(catalyst_window_t* window) {
    catalyst_destruct_window(window);
    CATALYST_DEALLOCATE(window);
}

void                catalyst_window_get_position(const catalyst_window_t* window, int* position_x, int* position_y) {
    window->get_position(window, position_x, position_y);
}

void                catalyst_window_set_position(const catalyst_window_t* window, int position_x, int position_y) {
    window->set_position(window, position_x, position_y);
}

void                catalyst_poll_events(const catalyst_window_t* window) {
    window->poll_events(window);
}
void                catalyst_wait_events(const catalyst_window_t* window) {
    window->wait_events(window);
}
bool                catalyst_window_close_requested(const catalyst_window_t* window) {
    return window->should_close(window);
}
bool                catalyst_window_get_key(const catalyst_window_t* window, const catalyst_key_t key) {
    return BITTEST(window->key_state, key);
}

bool                catalyst_window_get_mouse_button(const catalyst_window_t* window, const catalyst_mouse_button_t button) {
    return BITTEST(window->mouse_button_state, button);
}

void                catalyst_window_close(const catalyst_window_t* window) {
    ((catalyst_window_t *)window)->close_requested = true;
}

void                catalyst_window_minimize(const catalyst_window_t* window) {
    reset_window_events((catalyst_window_t*)window);
    window->minimize(window);
}

void                catalyst_window_maximize(const catalyst_window_t* window) {
    reset_window_events((catalyst_window_t*)window);
    window->maximize(window);
}

void                catalyst_window_restore(const catalyst_window_t* window) {
    reset_window_events((catalyst_window_t*)window);
    window->restore(window);
}

void                catalyst_window_show(const catalyst_window_t* window) {
    reset_window_events((catalyst_window_t *)window);
    window->show(window);
}

void                catalyst_window_hide(const catalyst_window_t* window) {
    reset_window_events((catalyst_window_t *)window);
    window->hide(window);
}

void                catalyst_window_focus(const catalyst_window_t* window) {
    window->focus(window);
}

void                catalyst_window_request_attention(const catalyst_window_t* window) {
    window->request_attention(window);
}

void                reset_window_events(catalyst_window_t* window) {
    for(unsigned i = 0; i < 32; i++)
        window->key_state[i] = 0;
    window->mouse_button_state[0] = 0;
}