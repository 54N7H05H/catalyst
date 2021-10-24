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
#ifndef CATALYST_INCLUDE__WINDOW_H_
#define CATALYST_INCLUDE__WINDOW_H_

#include <catalyst/compiler.h>

typedef struct catalyst_window_t catalyst_window_t;

typedef enum {
    CATALYST_WINDOW_ATTRIBUTE_HIDE              =   BITFIELD(0),

    CATALYST_WINDOW_ATTRIBUTE_RESIZABLE         =   BITFIELD(1),
    CATALYST_WINDOW_ATTRIBUTE_DECORATED         =   BITFIELD(2),
    CATALYST_WINDOW_ATTRIBUTE_FLOATING          =   BITFIELD(3),

    CATALYST_WINDOW_ATTRIBUTE_MAXIMIZED         =   BITFIELD(4),
    CATALYST_WINDOW_ATTRIBUTE_MINIMIZED         =   BITFIELD(5),

    CATALYST_WINDOW_ATTRIBUTE_FULLSCREEN        =   BITFIELD(6),
    CATALYST_WINDOW_ATTRIBUTE_OPENGL_CONTEXT    =   BITFIELD(7)
} catalyst_window_attribute_t;

typedef enum {
    CATALYST_KEY_NONE           =   0x00,   //  None of the key 

    CATALYST_KEY_A              =   0x04,   //  Keyboard a or A
    CATALYST_KEY_B              =   0x05,   //  Keyboard b or B
    CATALYST_KEY_C              =   0x06,   //  Keyboard c or C
    CATALYST_KEY_D              =   0x07,   //  Keyboard d or D
    CATALYST_KEY_E              =   0x08,   //  Keyboard e or E
    CATALYST_KEY_F              =   0x09,   //  Keyboard f or F
    CATALYST_KEY_G              =   0x0A,   //  Keyboard g or G
    CATALYST_KEY_H              =   0x0B,   //  Keyboard h or H
    CATALYST_KEY_I              =   0x0C,   //  Keyboard i or I
    CATALYST_KEY_J              =   0x0D,   //  Keyboard j or J
    CATALYST_KEY_K              =   0x0E,   //  Keyboard k or K
    CATALYST_KEY_L              =   0x0F,   //  Keyboard l or L
    CATALYST_KEY_M              =   0x10,   //  Keyboard m or M
    CATALYST_KEY_N              =   0x11,   //  Keyboard n or N
    CATALYST_KEY_O              =   0x12,   //  Keyboard o or O
    CATALYST_KEY_P              =   0x13,   //  Keyboard p or P
    CATALYST_KEY_Q              =   0x14,   //  Keyboard q or Q
    CATALYST_KEY_R              =   0x15,   //  Keyboard r or R
    CATALYST_KEY_S              =   0x16,   //  Keyboard s or S
    CATALYST_KEY_T              =   0x17,   //  Keyboard t or T
    CATALYST_KEY_U              =   0x18,   //  Keyboard u or U
    CATALYST_KEY_V              =   0x19,   //  Keyboard v or V
    CATALYST_KEY_W              =   0x1A,   //  Keyboard w or W
    CATALYST_KEY_X              =   0x1B,   //  Keyboard x or X
    CATALYST_KEY_Y              =   0x1C,   //  Keyboard y or Y
    CATALYST_KEY_Z              =   0x1D,   //  Keyboard z or Z

    CATALYST_KEY_1              =   0x1E,   //  Keyboard 1 or !
    CATALYST_KEY_2              =   0x1F,   //  Keyboard 2 or @
    CATALYST_KEY_3              =   0x20,   //  Keyboard 3 or #
    CATALYST_KEY_4              =   0x21,   //  Keyboard 4 or $
    CATALYST_KEY_5              =   0x22,   //  Keyboard 5 or %
    CATALYST_KEY_6              =   0x23,   //  Keyboard 6 or ^
    CATALYST_KEY_7              =   0x24,   //  Keyboard 7 or &
    CATALYST_KEY_8              =   0x25,   //  Keyboard 8 or *
    CATALYST_KEY_9              =   0x26,   //  Keyboard 9 or (
    CATALYST_KEY_0              =   0x27,   //  Keyboard 0 or )

    CATALYST_KEY_ENTER          =   0x28,   //  Keyboard Return / Enter Key
    CATALYST_KEY_ESCAPE         =   0x29,   //  Keyboard Escape key
    CATALYST_KEY_BACKSPACE      =   0x2A,   //  Keyboard backspace
    CATALYST_KEY_TAB            =   0x2B,   //  Keyboard Horizonatal Tab
    CATALYST_KEY_SPACE          =   0x2C,   //  Keyboard Spacebar
    CATALYST_KEY_MINUS          =   0x2D,   //  Keyboard - (MINUS) or _ (UNDERSCORE)
    CATALYST_KEY_EQUAL          =   0x2E,   //  Keyboard = (EQUAL) or + (PLUS)
    CATALYST_KEY_LEFT_BRACE     =   0x2F,   //  Keyboard { (LEFT_BRACE) or [ (LEFT_SQUARE)
    CATALYST_KEY_RIGHT_BRACE    =   0x30,   //  Keyboard } (RIGHT_BRACE) or ] (RIGHT_SQUARE)
    CATALYST_KEY_BACKSLASH      =   0x31,   //  Keyboard \ (BACKSLASH)
    CATALYST_KEY_SEMICOLON      =   0x33,   //  Keyboard ; (SEMICOLON)
    CATALYST_KEY_APOSTROPHE     =   0x34,   //  Keyboard ' (APOSTROPHE) or " (QUOTE)
    CATALYST_KEY_GRAVE_ACCENT   =   0x35,   //  Keyboard ` (GRAVE_ACCENT) or ~ (TILDE)
    CATALYST_KEY_COMMA          =   0x36,   //  Keyboard , (COMMA) or < (LEFT_ANGULAR)
    CATALYST_KEY_PERIOD         =   0x37,   //  Keyboard . (PERIOD) or > (RIGHT_ANGULAR)
    CATALYST_KEY_SLASH          =   0x38,   //  Keyboard / (SLASH) or ? (QUESTION)
    CATALYST_KEY_CAPSLOCK       =   0x39,   //  Keyboard CAPSLOCK

    CATALYST_KEY_F1             =   0x3A,   //  Keyboard F1
    CATALYST_KEY_F2             =   0x3B,   //  Keyboard F2
    CATALYST_KEY_F3             =   0x3C,   //  Keyboard F3
    CATALYST_KEY_F4             =   0x3D,   //  Keyboard F4
    CATALYST_KEY_F5             =   0x3E,   //  Keyboard F5
    CATALYST_KEY_F6             =   0x3F,   //  Keyboard F6
    CATALYST_KEY_F7             =   0x40,   //  Keyboard F7
    CATALYST_KEY_F8             =   0x41,   //  Keyboard F8
    CATALYST_KEY_F9             =   0x42,   //  Keyboard F9
    CATALYST_KEY_F10            =   0x43,   //  Keyboard F10
    CATALYST_KEY_F11            =   0x44,   //  Keyboard F11
    CATALYST_KEY_F12            =   0x45,   //  Keyboard F12

    CATALYST_KEY_SYSRQ          =   0x46,   //  Keyboard PRINT_SCREEN or SYSTEM_REQUIREMENTS
    CATALYST_KEY_SCROLL_LOCK    =   0x47,   //  Keyboard SCROLL_LOCK
    CATALYST_KEY_PAUSE          =   0x48,   //  Keyboard PAUSE
    CATALYST_KEY_INSERT         =   0x49,   //  Keyboard INSERT
    CATALYST_KEY_HOME           =   0x4A,   //  Keyboard HOME
    CATALYST_KEY_PAGE_UP        =   0x4B,   //  Keyboard PAGE_UP
    CATALYST_KEY_DELETE         =   0x4C,   //  Keyboard DELETE
    CATALYST_KEY_END            =   0x4D,   //  Keyboard END
    CATALYST_KEY_PAGE_DOWN      =   0x4E,   //  Keyboard PAGE_DOWN
    CATALYST_KEY_RIGHT          =   0x4F,   //  Keyboard RIGHT
    CATALYST_KEY_LEFT           =   0x50,   //  Keyboard LEFT
    CATALYST_KEY_DOWN           =   0x51,   //  Keyboard DOWN
    CATALYST_KEY_UP             =   0x52,   //  Keyboard UP

    CATALYST_KEY_NUM_LOCK       =   0x53,   //  Keyboard NUMLOCK
    CATALYST_KEY_KEYPAD_SLASH   =   0x54,   //  Keypad SLASH
    CATALYST_KEY_KEYPAD_ASTERISK=   0x55,   //  Keypad ASTERISK
    CATALYST_KEY_KEYPAD_MINUS   =   0x56,   //  Keypad MINUS
    CATALYST_KEY_KEYPAD_PLUS    =   0x57,   //  Keypad PLUS
    CATALYST_KEY_KEYPAD_ENTER   =   0x58,   //  Keypad Enter

    CATALYST_KEY_KEYPAD_1       =   0x59,   //  Keypad 1
    CATALYST_KEY_KEYPAD_2       =   0x5A,   //  Keypad 2
    CATALYST_KEY_KEYPAD_3       =   0x5B,   //  Keypad 3
    CATALYST_KEY_KEYPAD_4       =   0x5C,   //  Keypad 4
    CATALYST_KEY_KEYPAD_5       =   0x5D,   //  Keypad 5
    CATALYST_KEY_KEYPAD_6       =   0x5E,   //  Keypad 6
    CATALYST_KEY_KEYPAD_7       =   0x5F,   //  Keypad 7
    CATALYST_KEY_KEYPAD_8       =   0x60,   //  Keypad 8
    CATALYST_KEY_KEYPAD_9       =   0x61,   //  Keypad 9
    CATALYST_KEY_KEYPAD_0       =   0x62,   //  Keypad 0

    CATALYST_KEY_KEYPAD_PERIOD  =   0x63,   //  Keypad . or DELETE

    CATALYST_KEY_KEYPAD_102ND   =   0x64,   //  ![NON-US] Keyboard \ or |
    CATALYST_KEY_KEYPAD_COMPOSE =   0x65,   //  Keyboard Application
    CATALYST_KEY_KEYPAD_POWER   =   0x66,   //  Keyboard Power
    CATALYST_KEY_KEYPAD_EQUAL   =   0x67,   //  Keypad EQUAL

    CATALYST_KEY_F13            =   0x68,   //  Keyboard F13
    CATALYST_KEY_F14            =   0x69,   //  Keyboard F14
    CATALYST_KEY_F15            =   0x6A,   //  Keyboard F15
    CATALYST_KEY_F16            =   0x6B,   //  Keyboard F16
    CATALYST_KEY_F17            =   0x6C,   //  Keyboard F17
    CATALYST_KEY_F18            =   0x6D,   //  Keyboard F18
    CATALYST_KEY_F19            =   0x6E,   //  Keyboard F19
    CATALYST_KEY_F20            =   0x6F,   //  Keyboard F20
    CATALYST_KEY_F21            =   0x70,   //  Keyboard F21
    CATALYST_KEY_F22            =   0x71,   //  Keyboard F22
    CATALYST_KEY_F23            =   0x72,   //  Keyboard F23
    CATALYST_KEY_F24            =   0x73,   //  Keyboard F24

    CATALYST_KEY_LEFT_CONTROL   =   0xe0,
    CATALYST_KEY_LEFT_SHIFT     =   0xe1,
    CATALYST_KEY_LEFT_ALT       =   0xe2,
    CATALYST_KEY_LEFT_SUPER     =   0xe3,

    CATALYST_KEY_RIGHT_CONTROL  =   0xe4,
    CATALYST_KEY_RIGHT_SHIFT    =   0xe5,
    CATALYST_KEY_RIGHT_ALT      =   0xe6,
    CATALYST_KEY_RIGHT_SUPER    =   0xe7,

} catalyst_key_t;

typedef enum {
    CATALYST_MOUSE_BUTTON_1     =   0x00,
    CATALYST_MOUSE_BUTTON_2     =   0x01,
    CATALYST_MOUSE_BUTTON_3     =   0x02,
    CATALYST_MOUSE_BUTTON_4     =   0x03,
    CATALYST_MOUSE_BUTTON_5     =   0x04,
    CATALYST_MOUSE_BUTTON_6     =   0x05,
    CATALYST_MOUSE_BUTTON_7     =   0x06,
    CATALYST_MOUSE_BUTTON_8     =   0x07,

    CATALYST_MOUSE_BUTTON_LEFT  =   CATALYST_MOUSE_BUTTON_1,
    CATALYST_MOUSE_BUTTON_RIGHT =   CATALYST_MOUSE_BUTTON_2,
    CATALYST_MOUSE_BUTTON_MIDDLE=   CATALYST_MOUSE_BUTTON_3
} catalyst_mouse_button_t;

void                catalyst_initialize_window_subsystem();
void                catalyst_terminate_window_subsystem();

catalyst_window_t*  catalyst_create_window(
                                                int window_x, int winndow_y,
                                                int width, int height,
                                                const char* window_title,
                                                catalyst_window_attribute_t attributes
                                          );
void                catalyst_poll_events(const catalyst_window_t* window);
void                catalyst_wait_events(const catalyst_window_t* window);
bool                catalyst_window_close_requested(const catalyst_window_t* window);
bool                catalyst_window_get_key(const catalyst_window_t* window, const catalyst_key_t key);
bool                catalyst_window_get_mouse_button(const catalyst_window_t* window, const catalyst_mouse_button_t button);
void                catalyst_window_get_position(const catalyst_window_t* window, int* position_x, int* position_y);
void                catalyst_window_set_position(const catalyst_window_t* window, int position_x, int position_y);
void                catalyst_window_close(const catalyst_window_t* window);
void                catalyst_window_minimize(const catalyst_window_t* window);
void                catalyst_window_maximize(const catalyst_window_t* window);
void                catalyst_window_restore(const catalyst_window_t* window);
void                catalyst_window_show(const catalyst_window_t* window);
void                catalyst_window_hide(const catalyst_window_t* window);
void                catalyst_window_focus(const catalyst_window_t* window);
void                catalyst_window_request_attention(const catalyst_window_t* window);
void                catalyst_delete_window(catalyst_window_t* window);

#endif