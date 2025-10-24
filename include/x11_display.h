#ifndef X11_DISPLAY
#define X11_DISPLAY

#include <stdint.h>

int x11_initialize_display();

/* Returns -1 if the window wants to close */
int x11_update_display(uint8_t display_buffer[]);

int x11_close_display();

#endif