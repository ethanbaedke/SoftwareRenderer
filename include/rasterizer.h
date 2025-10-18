#ifndef RASTERIZER
#define RASTERIZER

#include <stdint.h>

int rasterize_screen_space_vertex_buffer(float screen_space_vertex_buffer[], uint16_t size, uint8_t display_buffer[]);

#endif