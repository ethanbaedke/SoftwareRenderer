#include <stdint.h>

#include "console_display.h"
#include "rasterizer.h"

// Buffer holding the final pixel color data
uint8_t display_buffer[3 * 64 * 36];

float screen_space_vertex_buffer[] =
{
    0.5, 0.25,
    0.25, 0.75,
    0.75, 0.75,
};

int main()
{
    initialize_display();

    for (uint16_t fakeLoop = 0; fakeLoop < 10000; fakeLoop++)
    {
        rasterize_screen_space_vertex_buffer(screen_space_vertex_buffer, sizeof(screen_space_vertex_buffer), display_buffer);
        write_to_display(display_buffer);
    }

    close_display();
    return 0;
}