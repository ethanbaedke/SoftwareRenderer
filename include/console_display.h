#ifndef CONSOLE_DISPLAY_H
#define CONSOLE_DISPLAY_H

#include <stdint.h>

int console_initialize_display();
int console_update_display(uint8_t display_buffer[]);
int console_close_display();

#endif