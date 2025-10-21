#include "rasterizer.h"

#include <math.h>
#include <float.h>

#include "constants.h"

int fill_pixel(uint16_t x, uint16_t y, uint8_t display_buffer[]);
float clamp_to_screen(float value);

int rasterize_screen_space_vertex_buffer(float screen_space_vertex_buffer[], uint16_t size, uint8_t display_buffer[])
{
    uint16_t num_triangles = (size / sizeof(float)) / 6;

    for (uint16_t i = 0; i < num_triangles; i++)
    {
        uint16_t triangle_start = i * 6;
        
        // Get the vertices of the triangle
        float ss_v1x = screen_space_vertex_buffer[triangle_start];
        float ss_v1y = screen_space_vertex_buffer[triangle_start + 1];

        float ss_v2x = screen_space_vertex_buffer[triangle_start + 2];
        float ss_v2y = screen_space_vertex_buffer[triangle_start + 3];

        float ss_v3x = screen_space_vertex_buffer[triangle_start + 4];
        float ss_v3y = screen_space_vertex_buffer[triangle_start + 5];

        // Get the bounding box of the triangle
        uint16_t px_left_bound = clamp_to_screen(fminf(ss_v1x, fminf(ss_v2x, ss_v3x))) * DISPLAY_WIDTH;
        uint16_t px_right_bound = clamp_to_screen(fmaxf(ss_v1x, fmaxf(ss_v2x, ss_v3x))) * DISPLAY_WIDTH;
        uint16_t px_top_bound = clamp_to_screen(fminf(ss_v1y, fminf(ss_v2y, ss_v3y))) * DISPLAY_HEIGHT;
        uint16_t px_bottom_bound = clamp_to_screen(fmaxf(ss_v1y, fmaxf(ss_v2y, ss_v3y))) * DISPLAY_HEIGHT;

        for (uint16_t y = px_top_bound; y <= px_bottom_bound; y++)
        {
            for (uint16_t x = px_left_bound; x <= px_right_bound; x++)
            {
                float px = ((float)x / DISPLAY_WIDTH) + HALF_PIXEL_SCREEN_WIDTH;
                float py = ((float)y / DISPLAY_HEIGHT) + HALF_PIXEL_SCREEN_HEIGHT;
                float v1p_x = px - ss_v1x;
                float v1p_y = py - ss_v1y;

                float v1v2_x = ss_v2x - ss_v1x;
                float v1v2_y = ss_v2y - ss_v1y;
                float v1v3_x = ss_v3x - ss_v1x;
                float v1v3_y = ss_v3y - ss_v1y;

                float area = 0.5f * ((v1v3_x * v1v2_y) - (v1v3_y * v1v2_x));
                float area_b = 0.5f * ((v1v3_x * v1p_y) - (v1v3_y * v1p_x));
                float area_c = 0.5f * ((v1v2_y * v1p_x) - (v1v2_x * v1p_y));
                float b = area_b / area;
                float c = area_c / area;
                float a = (1.0f - b) - c;

                if (a >= 0.0f && a <= 1.0f && b >= 0.0f && b <= 1.0f && c >= 0.0f && c <= 1.0f)
                {
                    fill_pixel(x, y, display_buffer);
                }
            }
        }
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

// Clamps values to the range [0, 1)
float clamp_to_screen(float value)
{
    return fminf(fmaxf(value, 0.0f), 1.0f - FLT_EPSILON);
}