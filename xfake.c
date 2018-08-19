#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>

// https://gist.github.com/pioz/726474

// gcc xfake.c -o xfake -lX11

static void
move (Display *display, int x, int y)
{
  XWarpPointer (display, None, None, 0,0,0,0, x, y);
  XFlush (display);
  usleep (1);
}

static void
click (Display *display, int button, int press, int release)
{
   // Create and setting up the event
   XEvent event;
   memset (&event, 0, sizeof (event));
   event.xbutton.button = button;
   event.xbutton.same_screen = True;
   event.xbutton.subwindow = DefaultRootWindow (display);
   while (event.xbutton.subwindow)
     {
        event.xbutton.window = event.xbutton.subwindow;
        XQueryPointer (display, event.xbutton.window,
              &event.xbutton.root, &event.xbutton.subwindow,
              &event.xbutton.x_root, &event.xbutton.y_root,
              &event.xbutton.x, &event.xbutton.y,
              &event.xbutton.state);
     }
   if (press)
     {
        // Press
        event.type = ButtonPress;
        if (XSendEvent (display, PointerWindow, True, ButtonPressMask, &event) == 0)
           fprintf (stderr, "Error to send the event!\n");
        XFlush (display);
        usleep (1);
     }
   if (release)
     {
        // Release
        event.type = ButtonRelease;
        if (XSendEvent (display, PointerWindow, True, ButtonReleaseMask, &event) == 0)
           fprintf (stderr, "Error to send the event!\n");
        XFlush (display);
        usleep (1);
     }
}

int main()
{
   // Obtain the X11 display.
   Display *display = XOpenDisplay(0);
   if(display == NULL)
      return -1;

   // Get the root window for the current display.
   Window winRoot = XDefaultRootWindow(display);

   click(display, 1, 1, 0);
   move(display, 50, 50);
   click(display, 1, 0, 1);

   // Done.
   XCloseDisplay(display);
   return 0;
}
