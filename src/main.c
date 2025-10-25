#include <stdint.h>
#include <unistd.h> // TESTING ONLY: used for sleep()
#include <stdlib.h> // For malloc()

#include "constants.h"
#include "logger.h"
#include "x11_display.h"
#include "rasterizer.h"

// Buffer holding the final pixel color data
uint8_t *display_buffer;

int application_entry_point();
void application_cleanup();
void test_rotate_triangle();

float screen_space_vertex_buffer[] =
{
    0.5, 0.25,
    0.25, 0.75,
    0.75, 0.75,
};

int main()
{
    log_message("Starting Software Renderer.");
    application_entry_point();

    application_cleanup();
    log_message("Closing Software Renderer.");

    return 0;
}

int application_entry_point()
{
    // Initialize display buffer
    display_buffer = malloc(3 * DISPLAY_WIDTH * DISPLAY_HEIGHT);

    // Initialize x11 display
    if (x11_initialize_display() == -1)
    {
        log_error("X11 display failed to initialize.");
        return -1;
    }

    // Core loop
    log_message("Starting core loop.");
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
    log_message("Core loop exited.");

    return 0;
}

void application_cleanup()
{
    x11_close_display();

    free(display_buffer);
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