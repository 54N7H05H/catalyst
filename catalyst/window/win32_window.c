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
#include <catalyst/window.h>
#include <catalyst/internal/window.h>

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#   define VC_EXTRALEAN
#endif

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN
#endif

#ifndef UNICODE
#   define UNICODE
#endif

#if WINVER < 0x0501
#   undef WINVER
#   define WINVER 0x0501
#endif
#if _WIN32_WINNT < 0x0501
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif
#define DIRECTINPUT_VERSION 0x0800

#define OEMRESOURCE

//  Prior to Windows SDK version 2104 (10.0.20348.0)
#pragma warning(disable:5105)   //  macro expansion producing 'defined' has undefined behavior
#   include <wctype.h>
#   include <windowsx.h>
#   include <dinput.h>
#   include <xinput.h>
#   include <dbt.h>
#pragma warning(disable:5105)   //  macro expansion producing 'defined' has undefined behavior

typedef struct catalyst_native_window_t {
    HWND                        window_handle;
    HINSTANCE                   instance_handle;
    HMODULE                     module_handle;
} catalyst_native_window_t;

static LPCWSTR CATALYST_WINDOW_WIN32_CLASS_NAME = L"CatalystWin32WindowClass";
static LRESULT CALLBACK win32_window_procedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);

void    win32_poll_events(const catalyst_window_t* window);
void    win32_wait_events(const catalyst_window_t* window);
bool    win32_get_key(const catalyst_window_t* window, const catalyst_key_t key);
bool    win32_get_mouse_button_state(const catalyst_window_t* window, const catalyst_mouse_button_t button);
bool    win32_window_should_close(const catalyst_window_t* window);
void    win32_window_get_position(const catalyst_window_t* window, int* position_x, int* position_y);
void    win32_window_set_position(const catalyst_window_t* window, int position_x, int position_y);
void    win32_window_minimize(const catalyst_window_t* window);
void    win32_window_maximize(const catalyst_window_t* window);
void    win32_window_restore(const catalyst_window_t* window);
void    win32_window_show(const catalyst_window_t* window);
void    win32_window_hide(const catalyst_window_t* window);
void    win32_window_focus(const catalyst_window_t* window);
void    win32_window_request_attention(const catalyst_window_t* window);

static catalyst_key_t translate[0x160] = {
    CATALYST_KEY_NONE,

    //  ESC 1 2 3 4 5 6 7 8 9 0 - = BACKSPACE
    CATALYST_KEY_ESCAPE,
    CATALYST_KEY_1,
    CATALYST_KEY_2,
    CATALYST_KEY_3,
    CATALYST_KEY_4,
    CATALYST_KEY_5,
    CATALYST_KEY_6,
    CATALYST_KEY_7,
    CATALYST_KEY_8,
    CATALYST_KEY_9,
    CATALYST_KEY_0,
    CATALYST_KEY_MINUS,
    CATALYST_KEY_EQUAL,
    CATALYST_KEY_BACKSPACE,

    //  TAB Q W E R T Y U I O P { } ENTER
    CATALYST_KEY_TAB,
    CATALYST_KEY_Q,
    CATALYST_KEY_W,
    CATALYST_KEY_E,
    CATALYST_KEY_R,
    CATALYST_KEY_T,
    CATALYST_KEY_Y,
    CATALYST_KEY_U,
    CATALYST_KEY_I,
    CATALYST_KEY_O,
    CATALYST_KEY_P,
    CATALYST_KEY_LEFT_BRACE,
    CATALYST_KEY_RIGHT_BRACE,
    CATALYST_KEY_ENTER,

    CATALYST_KEY_LEFT_CONTROL,

    //  A S D F G H J K L ; '
    CATALYST_KEY_A,
    CATALYST_KEY_S,
    CATALYST_KEY_D,
    CATALYST_KEY_F,
    CATALYST_KEY_G,
    CATALYST_KEY_H,
    CATALYST_KEY_J,
    CATALYST_KEY_K,
    CATALYST_KEY_L,
    CATALYST_KEY_SEMICOLON,
    CATALYST_KEY_APOSTROPHE,

    CATALYST_KEY_GRAVE_ACCENT,
    CATALYST_KEY_LEFT_SHIFT,
    CATALYST_KEY_BACKSLASH,

    //  Z X C V B N M , . /
    CATALYST_KEY_Z,
    CATALYST_KEY_X,
    CATALYST_KEY_C,
    CATALYST_KEY_V,
    CATALYST_KEY_B,
    CATALYST_KEY_N,
    CATALYST_KEY_M,
    CATALYST_KEY_COMMA,
    CATALYST_KEY_PERIOD,
    CATALYST_KEY_SLASH,

    CATALYST_KEY_RIGHT_SHIFT,
    CATALYST_KEY_KEYPAD_ASTERISK,   
    CATALYST_KEY_LEFT_ALT,

    CATALYST_KEY_SPACE,
    CATALYST_KEY_CAPSLOCK,

    CATALYST_KEY_F1,
    CATALYST_KEY_F2,
    CATALYST_KEY_F3,
    CATALYST_KEY_F4,
    CATALYST_KEY_F5,
    CATALYST_KEY_F6,
    CATALYST_KEY_F7,
    CATALYST_KEY_F8,
    CATALYST_KEY_F9,
    CATALYST_KEY_F10,

    CATALYST_KEY_PAUSE,
    CATALYST_KEY_SCROLL_LOCK,

    CATALYST_KEY_KEYPAD_7,
    CATALYST_KEY_KEYPAD_8,
    CATALYST_KEY_KEYPAD_9,
    CATALYST_KEY_KEYPAD_MINUS,
    CATALYST_KEY_KEYPAD_4,
    CATALYST_KEY_KEYPAD_5,
    CATALYST_KEY_KEYPAD_6,
    CATALYST_KEY_KEYPAD_PLUS,
    CATALYST_KEY_KEYPAD_1,
    CATALYST_KEY_KEYPAD_2,
    CATALYST_KEY_KEYPAD_3,
    CATALYST_KEY_KEYPAD_0,
    CATALYST_KEY_KEYPAD_PERIOD,

    CATALYST_KEY_NONE,
    CATALYST_KEY_NONE,

    CATALYST_KEY_NONE,      //  TODO Assign World 2 here

    CATALYST_KEY_F11,
    CATALYST_KEY_F12,

    CATALYST_KEY_KEYPAD_EQUAL,

    CATALYST_KEY_NONE,
    CATALYST_KEY_NONE,
    CATALYST_KEY_NONE,
    CATALYST_KEY_NONE,

    CATALYST_KEY_F13,
    CATALYST_KEY_F14,
    CATALYST_KEY_F15,
    CATALYST_KEY_F16,
    CATALYST_KEY_F17,
    CATALYST_KEY_F18,
    CATALYST_KEY_F19,
    CATALYST_KEY_F20,
    CATALYST_KEY_F21,
    CATALYST_KEY_F22,
    CATALYST_KEY_F23,

    CATALYST_KEY_NONE, CATALYST_KEY_NONE, CATALYST_KEY_NONE, CATALYST_KEY_NONE, CATALYST_KEY_NONE, CATALYST_KEY_NONE,CATALYST_KEY_NONE,

    CATALYST_KEY_F24,
};

static DWORD win32_get_window_style(catalyst_window_attribute_t attributes);
static DWORD win32_get_window_extended_style(catalyst_window_attribute_t attributes);
static void  win32_get_window_size(DWORD window_style, DWORD window_extended_style, int width, int height,
                                    int* full_width, int* full_height);
static WCHAR* win32_create_wide_from_utf8(const char* string);
static void   win32_delete_wide_from_utf8(WCHAR* wide_string);

void    catalyst_initialize_window_subsystem() {
    //  Initialize rest of the keyboard scancodes
    translate[ 0x11D ] = CATALYST_KEY_RIGHT_CONTROL;
    translate[ 0x135 ] = CATALYST_KEY_KEYPAD_SLASH;
    translate[ 0x137 ] = CATALYST_KEY_SYSRQ;
    translate[ 0x138 ] = CATALYST_KEY_RIGHT_ALT;
    translate[ 0x145 ] = CATALYST_KEY_NUM_LOCK;
    translate[ 0x146 ] = CATALYST_KEY_PAUSE;
    translate[ 0x147 ] = CATALYST_KEY_HOME;
    translate[ 0x148 ] = CATALYST_KEY_UP;
    translate[ 0x149 ] = CATALYST_KEY_PAGE_UP;
    translate[ 0x14b ] = CATALYST_KEY_LEFT;
    translate[ 0x14d ] = CATALYST_KEY_RIGHT;
    translate[ 0x14f ] = CATALYST_KEY_END;
    translate[ 0x150 ] = CATALYST_KEY_DOWN;
    translate[ 0x151 ] = CATALYST_KEY_PAGE_DOWN;
    translate[ 0x152 ] = CATALYST_KEY_INSERT;
    translate[ 0x153 ] = CATALYST_KEY_DELETE;


    //  Register Win32 Window Class
    WNDCLASSEXW window_class;
    ZeroMemory(&window_class, sizeof (WNDCLASSEXW));

    window_class.cbSize         =   sizeof (WNDCLASSEXW);
    window_class.style          =   CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc    =   win32_window_procedure;
    window_class.hInstance      =   GetModuleHandleW(NULL);
    window_class.hCursor        =   LoadCursorW(NULL, IDC_ARROW);
    window_class.lpszClassName  =   CATALYST_WINDOW_WIN32_CLASS_NAME;

    if(!RegisterClassEx(&window_class))
        CATALYST_FATAL("Win32 : Failed to register native {.platform = Win32} window class");
}


void    catalyst_terminate_window_subsystem() {
    //  Unregister Win32 Window Class
    UnregisterClassW(CATALYST_WINDOW_WIN32_CLASS_NAME, GetModuleHandleW(NULL));
}

void    catalyst_construct_window(catalyst_window_t* window, int width, int height,
                                    const char* title, catalyst_window_attribute_t attributes) {

    DWORD   window_style = win32_get_window_style(attributes),
            window_extended_style = win32_get_window_extended_style(attributes);
    
    if(attributes & CATALYST_WINDOW_ATTRIBUTE_MAXIMIZED)
        window_style |= WS_MAXIMIZE;
    
    int full_width, full_height;
    win32_get_window_size(window_style, window_extended_style, width, height, &full_width, &full_height);

    HMODULE win32_module_handle = GetModuleHandleW(NULL);

    WCHAR* window_wide_title = win32_create_wide_from_utf8(title);
    window->native_window = CATALYST_ALLOCATE(catalyst_native_window_t);

    window->native_window->window_handle = CreateWindowExW(
        window_extended_style, CATALYST_WINDOW_WIN32_CLASS_NAME,
        window_wide_title,
        window_style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        full_width, full_height,
        NULL, FALSE, GetModuleHandleW(NULL),
        NULL
    );
    win32_delete_wide_from_utf8(window_wide_title);

    if(!window->native_window->window_handle)
        CATALYST_FATAL("Win32 : Failed to create a window. Issued to catalyst_window_t %p", window);
    
    window->native_window->module_handle    = GetModuleHandleW(NULL);
    window->attributes                      = attributes;
    window->close_requested                 = false;

    window->poll_events                     = win32_poll_events;
    window->wait_events                     = win32_wait_events;
    window->should_close                    = win32_window_should_close;
    window->get_position                    = win32_window_get_position;
    window->set_position                    = win32_window_set_position;
    window->minimize                        = win32_window_minimize;
    window->maximize                        = win32_window_maximize;
    window->restore                         = win32_window_restore;
    window->show                            = win32_window_show;
    window->hide                            = win32_window_hide;
    window->focus                           = win32_window_focus;
    window->request_attention               = win32_window_request_attention;
    
    SetPropW(window->native_window->window_handle, L"CATALYST_WINDOW", window);

    if(!(attributes & CATALYST_WINDOW_ATTRIBUTE_HIDE))
        ShowWindow(window->native_window->window_handle, SW_SHOW);
    
    CATALYST_TRACE("Win32: Successfully constructed native window. Issued to catalyst_window_t %p", window);
}

void    catalyst_destruct_window(catalyst_window_t* window) {
    if(window->native_window->window_handle != NULL) {
        DestroyWindow(window->native_window->window_handle);
        CATALYST_TRACE("Win32: Successfully destructed native window. Issued to catalyst_window_t %p", window);
    }
    CATALYST_DEALLOCATE(window->native_window);
}

//
//
//
WCHAR*  win32_create_wide_from_utf8(const char* string) {
    WCHAR* wide_string;
    int string_length;

    string_length = MultiByteToWideChar(CP_UTF8, 0, string, -1, NULL, 0);

    wide_string = CATALYST_ALLOCATE_ARRAY(WCHAR, string_length);
    MultiByteToWideChar(CP_UTF8, 0, string, -1, wide_string, string_length);

    return wide_string;
}


void    win32_delete_wide_from_utf8(WCHAR* wide_string) {
    CATALYST_DEALLOCATE(wide_string);
}

DWORD   win32_get_window_style(catalyst_window_attribute_t attributes) {
    DWORD win32_style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    if(attributes & CATALYST_WINDOW_ATTRIBUTE_FULLSCREEN)
        win32_style = WS_POPUP;
    else {
        win32_style = WS_SYSMENU | WS_MINIMIZEBOX;
        if(attributes & CATALYST_WINDOW_ATTRIBUTE_DECORATED) {
            win32_style |= WS_CAPTION;
            if(attributes & CATALYST_WINDOW_ATTRIBUTE_RESIZABLE)
                win32_style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
        } else
            win32_style |= WS_POPUP;
    }

    return win32_style;
}

DWORD   win32_get_window_extended_style(catalyst_window_attribute_t attributes) {
    DWORD win32_extended_style = WS_EX_APPWINDOW;
    if(attributes & CATALYST_WINDOW_ATTRIBUTE_FULLSCREEN || attributes & CATALYST_WINDOW_ATTRIBUTE_FLOATING)
        win32_extended_style = WS_EX_TOPMOST;
    
    return win32_extended_style;
}

void    win32_get_window_size(DWORD window_style, DWORD window_extended_style,
                                int width, int height,
                                int* full_width, int* full_height ) {
    RECT rect = { 0, 0, (LONG)width, (LONG)height };
    AdjustWindowRectEx(&rect, window_style, FALSE, window_extended_style);
    
    *full_width  =  rect.right - rect.left;
    *full_height =  rect.bottom - rect.top;
}

void    catalyst_win32_handle_key_event(catalyst_window_t* window, catalyst_key_t key, bool action);
//
//
//

void    win32_poll_events(const catalyst_window_t* window) {
    MSG message;

    while(PeekMessageW(&message, NULL, 0, 0, PM_REMOVE)) {
        if(message.message == WM_QUIT) {
            CATALYST_DEBUG("Win32 : Window quit event has been posted by Win32");
        } else {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    HWND window_handle = GetActiveWindow();
    if(window_handle) {
        catalyst_window_t* catalyst_window = GetPropW(window_handle, L"CATALYST_WINDOW");
        if(catalyst_window) {
            const int key_pair[4][2] = {
                {VK_LSHIFT, CATALYST_KEY_LEFT_SHIFT},
                {VK_RSHIFT, CATALYST_KEY_RIGHT_SHIFT},
                {VK_LWIN, CATALYST_KEY_LEFT_SUPER},
                {VK_RWIN, CATALYST_KEY_RIGHT_SUPER}
            };

            for(int i = 0; i < 4; ++i) {
                if((GetKeyState(key_pair[i][0] & 0x8000)) || window->key_state[key_pair[i][1]] != true)
                    continue;
                catalyst_win32_handle_key_event(catalyst_window, key_pair[i][1], false);
            }
        }
    }
}

void    win32_wait_events(const catalyst_window_t* window) {
    WaitMessage();
    win32_poll_events(window);
}

bool    win32_window_should_close(const catalyst_window_t* window) {
    return window->close_requested;
}

void    win32_window_get_position(const catalyst_window_t* window, int* position_x, int* position_y) {
    POINT position = {0, 0};
    ClientToScreen(window->native_window->window_handle, &position);
    if(position.x) *position_x = position.x;
    if(position.y) *position_y = position.y;
}

void    win32_window_set_position(const catalyst_window_t* window, int position_x, int position_y) {
    RECT rect = {position_x, position_y, position_x, position_y};
    AdjustWindowRectEx(&rect, win32_get_window_style(window->attributes), FALSE, win32_get_window_extended_style(window->attributes));
    SetWindowPos(window->native_window->window_handle, NULL, rect.left, rect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void    win32_window_minimize(const catalyst_window_t* window) {
    ShowWindow(window->native_window->window_handle, SW_MINIMIZE);
}

void    win32_window_maximize(const catalyst_window_t* window) {
    ShowWindow(window->native_window->window_handle, SW_MAXIMIZE);
}

void    win32_window_restore(const catalyst_window_t* window) {
    ShowWindow(window->native_window->window_handle, SW_RESTORE);
}

void    win32_window_show(const catalyst_window_t* window) {
    ShowWindow(window->native_window->window_handle, SW_SHOWNA);
}

void    win32_window_hide(const catalyst_window_t* window) {
    ShowWindow(window->native_window->window_handle, SW_HIDE);
}

void    win32_window_focus(const catalyst_window_t* window) {
    BringWindowToTop(window->native_window->window_handle);
    SetForegroundWindow(window->native_window->window_handle);
    SetFocus(window->native_window->window_handle);
}


void    win32_window_request_attention(const catalyst_window_t* window) {
    FlashWindow(window->native_window->window_handle, TRUE);
}
//
//
//
void catalyst_win32_get_key(WPARAM wparam, LPARAM lparam, catalyst_key_t* key) {
    int scancode;
    const int action = (HIWORD(lparam) & KF_UP) ? 0 : 1;
    
    scancode = (HIWORD(lparam) & (KF_EXTENDED | 0xff));
    if(!scancode)
        scancode = MapVirtualKeyW((UINT)wparam, MAPVK_VK_TO_VSC);
    *key = translate[scancode];
}


void    catalyst_win32_handle_key_event(catalyst_window_t* window, catalyst_key_t key, bool action) {
    bool key_repeat = false;
    if(action == false && BITTEST(window->key_state, key) == false)
        return ;
    if(action == true && BITTEST(window->key_state, key) == true)
        key_repeat = true;
    
    action ? BITSET(window->key_state, key) : BITCLEAR(window->key_state, key);
}


void    catalyst_win32_handle_mouse_click_event(catalyst_window_t* window, catalyst_mouse_button_t mouse_button, bool action) {
    if(action)
        BITSET(window->mouse_button_state, mouse_button);
    else
        BITCLEAR(window->mouse_button_state, mouse_button);
}
static LRESULT CALLBACK win32_window_procedure(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
    catalyst_window_t* window = GetPropW(window_handle, L"CATALYST_WINDOW");

    switch(message) {
        case WM_CLOSE:
            CATALYST_TRACE("Win32 : Close request has been sent to native window %p", window);
            window->close_requested = true;
            break;

        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
            {
                catalyst_mouse_button_t mouse_button;
                bool mouse_pressed = false;
                if(message == WM_LBUTTONDOWN || message == WM_LBUTTONUP)
                    mouse_button = CATALYST_MOUSE_BUTTON_LEFT;
                else if(message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
                    mouse_button = CATALYST_MOUSE_BUTTON_RIGHT;
                else if(message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
                    mouse_button = CATALYST_MOUSE_BUTTON_MIDDLE;
                else if(GET_XBUTTON_WPARAM(wparam) == XBUTTON1)
                    mouse_button = CATALYST_MOUSE_BUTTON_4;
                else
                    mouse_button = CATALYST_MOUSE_BUTTON_5;
                
                if(message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_XBUTTONDOWN)
                    mouse_pressed = true;
                
                catalyst_win32_handle_mouse_click_event(window, mouse_button, mouse_pressed);
                
                if(message == WM_XBUTTONDOWN || message == WM_XBUTTONUP)
                    return TRUE;
                else
                    return FALSE;
            }
        
        case WM_KEYUP:
        case WM_KEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
            {
                catalyst_key_t key;
                const bool key_pressed = (HIWORD(lparam) & KF_UP) ? false : true;
                catalyst_win32_get_key(wparam, lparam, &key);

                if(wparam == VK_CONTROL) {
                    if(HIWORD(lparam) & KF_EXTENDED)
                        key = CATALYST_KEY_RIGHT_CONTROL;
                    else {
                        MSG next_message;
                        const DWORD message_time = GetMessageTime();
                        if(PeekMessageW(&next_message, NULL, 0, 0, PM_NOREMOVE)) {
                            if( next_message.message == WM_KEYDOWN || next_message.message == WM_KEYUP ||
                                next_message.message == WM_SYSKEYDOWN || next_message.message == WM_SYSKEYUP) {
                                    if(next_message.wParam == VK_MENU && (HIWORD(next_message.lParam) & KF_EXTENDED) &&
                                        next_message.time == message_time)
                                            break;
                                }
                        }
                        key = CATALYST_KEY_LEFT_CONTROL;
                    }
                } else if (wparam == VK_PROCESSKEY)
                    break;
                else if(wparam == VK_SNAPSHOT) {
                    catalyst_win32_handle_key_event(window, CATALYST_KEY_SYSRQ, true);
                    catalyst_win32_handle_key_event(window, CATALYST_KEY_SYSRQ, false);
                } else if (wparam == VK_SHIFT && key_pressed == false) {
                    catalyst_win32_handle_key_event(window, CATALYST_KEY_LEFT_SHIFT, false);
                    catalyst_win32_handle_key_event(window, CATALYST_KEY_RIGHT_SHIFT, false);
                }
                catalyst_win32_handle_key_event(window, key, key_pressed);

                catalyst_win32_handle_key_event((catalyst_window_t *)window, CATALYST_KEY_LEFT_SUPER, GetKeyState(VK_LWIN));
                catalyst_win32_handle_key_event((catalyst_window_t *)window, CATALYST_KEY_RIGHT_SUPER, GetKeyState(VK_RWIN));
            }
            break;
    }
    return DefWindowProc(window_handle, message, wparam, lparam);
}