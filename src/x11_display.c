#include "x11_display.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "constants.h"
#include "logger.h"

Display *x_display;
Window x_window;
GC x_graphics_context;

int x11_initialize_display()
{
    log_message("Initializing X11 display.");

    // Connect to the server
    x_display = XOpenDisplay(NULL);
    if (x_display == NULL)
    {
        log_error("Could not open X11 display.");
        return -1;
    }

    // Black and white colors
    int x_black_color = BlackPixel(x_display, DefaultScreen(x_display));
    int x_white_color = WhitePixel(x_display, DefaultScreen(x_display));

    // Create the window
    x_window = XCreateSimpleWindow(x_display, DefaultRootWindow(x_display), 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, x_black_color, x_black_color);

    // Get MapNotify events
    XSelectInput(x_display, x_window, StructureNotifyMask | KeyPressMask);

    // Make the window appear on the screen
    XMapWindow(x_display, x_window);

    // Holds drawing parameters that need to be passed to the server on every draw request
    x_graphics_context = DefaultGC(x_display, DefaultScreen(x_display));

    // Wait for window to appear
    XEvent e;
    while (e.type != MapNotify)
    {
        XNextEvent(x_display, &e);
    }

    log_message("X11 display initialized.");
}

int x11_update_display(uint8_t display_buffer[])
{
    // Process any window events
    if (XPending(x_display))
    {
        XEvent e;
        XNextEvent(x_display, &e);

        if (e.type == KeyPress)
        {
            return -1;
        }
    }

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
}

int x11_close_display()
{
    if (x_display == NULL)
    {
        log_warning("Attempting to close non-existent X11 display.");
        return -1;
    }

    log_message("Closing X11 display.");

    XCloseDisplay(x_display);

    log_message("X11 display closed.");
}