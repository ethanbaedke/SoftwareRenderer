#include <stdio.h>

const int DISPLAY_WIDTH = 64;
const int DISPLAY_HEIGHT = 36;

int main()
{
    // Enable alternate screen buffer
    printf("\x1b[?1049h");
    fflush(stdout);

    // Clear space for rendering
    for (int y = 0; y < DISPLAY_HEIGHT; y++)
    {
        printf("\n");
    }

    for (int fakeLoop = 0; fakeLoop < 100000; fakeLoop++)
    {
        // Move to the top of display
        printf("\033[%dA\r", DISPLAY_HEIGHT);
        fflush(stdout);

        // Display image
        for (int y = 0; y < DISPLAY_HEIGHT; y++)
        {
            // Set rgb background color
            printf("\033[48;2;0;255;255m");

            // Print line
            printf("%*s", DISPLAY_WIDTH * 2, "");

            // Reset terminal to default color and move to new line
            printf("\033[0m\n");
        }
    }

    // Return to the default screen buffer
    printf("\x1b[?1049l");
    fflush(stdout);

    // Reset terminal to default and return
    printf("\033[0m\n");
    printf("Finished.\n");
    fflush(stdout);
    return 0;
}