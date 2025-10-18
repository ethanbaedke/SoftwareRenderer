#include "rasterizer.h"

#include <math.h>

#include "constants.h"

int fill_pixel(uint16_t x, uint16_t y, uint8_t display_buffer[]);

int rasterize_screen_space_vertex_buffer(float screen_space_vertex_buffer[], uint16_t size, uint8_t display_buffer[])
{
    uint16_t num_triangles = (size / sizeof(float)) / 6;

    for (uint16_t i = 0; i < num_triangles; i++)
    {
        uint16_t triangle_start = i * 6;
        
        float ss_v1x = screen_space_vertex_buffer[triangle_start];
        float ss_v1y = screen_space_vertex_buffer[triangle_start + 1];

        float ss_v2x = screen_space_vertex_buffer[triangle_start + 2];
        float ss_v2y = screen_space_vertex_buffer[triangle_start + 3];

        float ss_v3x = screen_space_vertex_buffer[triangle_start + 4];
        float ss_v3y = screen_space_vertex_buffer[triangle_start + 5];

        uint16_t px_left_bound = floorf(fminf(ss_v1x, fminf(ss_v2x, ss_v3x)) * DISPLAY_WIDTH);
        uint16_t px_right_bound = ceilf(fmaxf(ss_v1x, fmaxf(ss_v2x, ss_v3x)) * DISPLAY_WIDTH);
        uint16_t px_top_bound = floorf(fminf(ss_v1y, fminf(ss_v2y, ss_v3y)) * DISPLAY_HEIGHT);
        uint16_t px_bottom_bound = ceilf(fmaxf(ss_v1y, fmaxf(ss_v2y, ss_v3y)) * DISPLAY_HEIGHT);

        fill_pixel(px_left_bound, px_bottom_bound, display_buffer);
        fill_pixel(px_right_bound, px_top_bound, display_buffer);

        //uint16_t px_bounding_box_width = px_right_bound - px_left_bound;
        //uint16_t px_bounding_box_height = px_top_bound - px_bottom_bound;
    }

    return 0;
}

int fill_pixel(uint16_t x, uint16_t y, uint8_t display_buffer[])
{
    uint16_t start = ((y * DISPLAY_WIDTH) + x) * 3;
    display_buffer[start] = 255;
    display_buffer[start + 1] = 255;
    display_buffer[start + 2] = 255;

    return 0;
}