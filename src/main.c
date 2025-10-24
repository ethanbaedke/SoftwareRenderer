#include <stdint.h>
#include <stdio.h>
#include <unistd.h> // TESTING ONLY: used for sleep()
#include <stdlib.h> // For malloc()

#include "constants.h"
#include "x11_display.h"
#include "rasterizer.h"

// Buffer holding the final pixel color data
uint8_t *display_buffer;

void test_rotate_triangle();

float screen_space_vertex_buffer[] =
{
    0.5, 0.25,
    0.25, 0.75,
    0.75, 0.75,
};

int main()
{
    printf("Starting Software Renderer.\n");
    fflush(stdout);

    // Initialize display buffer
    display_buffer = malloc(3 * DISPLAY_WIDTH * DISPLAY_HEIGHT);

    // Initialize x11 display
    x11_initialize_display();

    // Core loop
    printf("Starting core loop.\n");
    fflush(stdout);
    while (1)
    {
        // TESTING
        test_rotate_triangle();

        // Rasterization
        rasterize_screen_space_vertex_buffer(screen_space_vertex_buffer, sizeof(screen_space_vertex_buffer), display_buffer);

        int x11_update_result = x11_update_display(display_buffer);
        if (x11_update_result == -1) // Window wants to close
        {
            break;
        }

        // Clear display buffer
        for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT * 3; i++)
        {
            display_buffer[i] = 0;
        }

        sleep(1.0f / 30.0f);
    }
    printf("Core loop exited.\n");
    fflush(stdout);

    x11_close_display();

    free(display_buffer);

    printf("Closing Software Renderer.\n");
    fflush(stdout);
    return 0;
}

//TESTING
#include <math.h>
float test_rotate_amount = 0.0f;
float spacing = -(3.14f * 2.0f) / 3.0f;
void test_rotate_triangle()
{
    screen_space_vertex_buffer[0] = cos(test_rotate_amount);
    screen_space_vertex_buffer[1] = sin(test_rotate_amount);

    screen_space_vertex_buffer[2] = cos(test_rotate_amount + spacing);
    screen_space_vertex_buffer[3] = sin(test_rotate_amount + spacing);

    screen_space_vertex_buffer[4] = cos(test_rotate_amount + (spacing * 2.0f));
    screen_space_vertex_buffer[5] = sin(test_rotate_amount + (spacing * 2.0f));

    for (int i = 0; i < 6; i++)
    {
        screen_space_vertex_buffer[i] = (screen_space_vertex_buffer[i] * 0.25f) + 0.5f;
    }

    test_rotate_amount += 0.01f;
}