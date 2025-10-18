#ifndef CONSOLE_DISPLAY_H
#define CONSOLE_DISPLAY_H

#include <stdint.h>

int initialize_display();
int write_to_display(uint8_t display_buffer[]);
int close_display();

#endif