#include "console_display.h"

#include <stdio.h>

#include "constants.h"

int initialize_display()
{
    // Enable alternate screen buffer
    printf("\033[?1049h");
    fflush(stdout);

    // Clear space for rendering
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        printf("\n");
    }

    return 0;
}

int write_to_display(uint8_t display_buffer[])
{
    // Move to the top of display
    printf("\033[%dA\r", DISPLAY_HEIGHT);
    fflush(stdout);

    // Display image
    uint16_t buffer_index = 0;
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (int x = 0; x < DISPLAY_WIDTH; x++)
        {
            // Get RGB of the current pixel from the buffer
            uint8_t pixel_r = display_buffer[buffer_index];
            buffer_index++;
            uint8_t pixel_g = display_buffer[buffer_index];
            buffer_index++;
            uint8_t pixel_b = display_buffer[buffer_index];
            buffer_index++;

            // Set the pixel color
            printf("\033[48;2;%d;%d;%dm", pixel_r, pixel_g, pixel_b);

            // Print pixel
            printf("  ");
        }

        // Reset terminal to default color and move to new line
        printf("\033[0m\n");
    }

    return 0;
}

int close_display()
{
    // Return to the default screen buffer
    printf("\033[?1049l");
    fflush(stdout);

    // Reset terminal to default and return
    printf("\033[0m\n");
    fflush(stdout);

    return 0;
}