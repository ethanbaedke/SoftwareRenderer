#include <stdint.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h> // TESTING ONLY: used for sleep()
#include <stdlib.h> // For malloc()

#include "constants.h"
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
    // Initialize display buffer
    display_buffer = malloc(3 * DISPLAY_WIDTH * DISPLAY_HEIGHT);

    printf("Setting up window.\n");

    // Connect to the server
    Display *x_display = XOpenDisplay(NULL);
    if (x_display == NULL)
    {
        printf("Could not open X11 display.\n");
        return -1;
    }

    // Black and white colors
    int x_black_color = BlackPixel(x_display, DefaultScreen(x_display));
    int x_white_color = WhitePixel(x_display, DefaultScreen(x_display));

    // Create the window
    Window x_window = XCreateSimpleWindow(x_display, DefaultRootWindow(x_display), 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, x_black_color, x_black_color);

    // Get MapNotify events
    XSelectInput(x_display, x_window, StructureNotifyMask | KeyPressMask);

    // Make the window appear on the screen
    XMapWindow(x_display, x_window);

    // Holds drawing parameters that need to be passed to the server on every draw request
    GC x_graphics_context = DefaultGC(x_display, DefaultScreen(x_display));

    // Wait for window to appear
    XEvent e;
    while (e.type != MapNotify)
    {
        XNextEvent(x_display, &e);
    }

    printf("Begin rendering.\n");

    // Render loop
    while (True)
    {
        // Process any window events
        if (XPending(x_display))
        {
            XNextEvent(x_display, &e);

            if (e.type == KeyPress)
            {
                break;
            }
        }

        // TESTING
        test_rotate_triangle();

        // Rasterization
        rasterize_screen_space_vertex_buffer(screen_space_vertex_buffer, sizeof(screen_space_vertex_buffer), display_buffer);

        // Display
        XImage *x_image = XGetImage(x_display, x_window, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, AllPlanes, ZPixmap);
        for (int y = 0; y < DISPLAY_HEIGHT; y++)
        {
            for (int x = 0; x < DISPLAY_WIDTH; x++)
            {
                int ind = ((y * DISPLAY_WIDTH) + x) * 3;
                XPutPixel(x_image, x, y, (display_buffer[ind] << 16) | (display_buffer[ind + 1] << 8) | display_buffer[ind + 2]);
            }
        }
        XPutImage(x_display, x_window, x_graphics_context, x_image, 0, 0, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

        // Clear display buffer
        for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT * 3; i++)
        {
            display_buffer[i] = 0;
        }
    }

    XCloseDisplay(x_display);
    printf("Window closed.\n");

    free(display_buffer);

    printf("Software Renderer exiting.\n");
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