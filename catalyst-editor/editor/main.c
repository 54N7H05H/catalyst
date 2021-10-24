#include <catalyst/logging.h>
#include <catalyst/memory.h>
#include <catalyst/window.h>

int main(int argc, char** argv) {
    catalyst_initialize_logging(); 
    catalyst_initialize_memory_subsystem();
    catalyst_initialize_window_subsystem();

    catalyst_window_t* window = catalyst_create_window(600, 400, 600, 400, "Catalyst エディター", CATALYST_WINDOW_ATTRIBUTE_DECORATED);
    while(!catalyst_window_close_requested(window)) {
        if(catalyst_window_get_key(window, CATALYST_KEY_Q)) {
            CATALYST_INFO("Quit");
            catalyst_window_close(window);   
        }
        
        if(catalyst_window_get_key(window, CATALYST_KEY_M)) {
            CATALYST_INFO("Maximizing window %p", window);
            catalyst_window_maximize(window);
        }

        if(catalyst_window_get_key(window, CATALYST_KEY_R)) {
            CATALYST_INFO("Restoring window %p", window);
            catalyst_window_restore(window);
        }

        if(catalyst_window_get_mouse_button(window, CATALYST_MOUSE_BUTTON_RIGHT)) {
            CATALYST_INFO("Flash Window %p", window);
            catalyst_window_request_attention(window);
        }

        catalyst_poll_events(window);
    }
    catalyst_delete_window(window);

    catalyst_terminate_window_subsystem();
    catalyst_terminate_memory_subsystem();
    catalyst_terminate_logging(); 
    
    return 0;
}
