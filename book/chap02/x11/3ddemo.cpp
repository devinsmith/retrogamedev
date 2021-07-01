///////////////////////////////////////////////////////////
// 3DDEMO.CPP: Simple 3-D graphics drawing demonatration.
//             This program uses X11.
///////////////////////////////////////////////////////////

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

// Function prototypes.
void StartGraphics(void);
void DrawGraphics(void);

// Globals, to mimic BGI
static Display *g_display;
static GC g_gc;
static Window g_win;
static int screen_num;
static long ltgray;
static long dkgray;

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  // Initialize VGA graphics.
  StartGraphics();

  // Wait for a key press.
  bool keepGoing = true;
  while (keepGoing) {
    XEvent ev;
    XNextEvent(g_display, &ev);
    switch (ev.type) {
    case ButtonPress:
      keepGoing = false;
      break;
    case KeyPress:
      keepGoing = false;
      break;
    case Expose:
      // Draw the 3-D graphics.
      DrawGraphics();
      break;
    }

  }

  XFreeGC(g_display, g_gc);
  XDestroyWindow(g_display, g_win);
  XCloseDisplay(g_display);
  return 1;
}

///////////////////////////////////////////////////////////
// DrawGraphics()
///////////////////////////////////////////////////////////
void DrawGraphics(void)
{
  // Set color.
  XSetForeground(g_display, g_gc, ltgray);

  // Draw a gray background rectangle.
  XFillRectangle(g_display, g_win, g_gc, 100, 100, 200, 140);

  // Draw the top, protruding 3-D rectangle.
  XSetForeground(g_display, g_gc, WhitePixel(g_display, screen_num));
  // Top
  XDrawLine(g_display, g_win, g_gc, 120, 120, 279, 120);
  // Left
  XDrawLine(g_display, g_win, g_gc, 120, 121, 120, 160);
  XSetForeground(g_display, g_gc, BlackPixel(g_display, screen_num));
  // Right
  XDrawLine(g_display, g_win, g_gc, 280, 121, 280, 160);
  // Bottom
  XDrawLine(g_display, g_win, g_gc, 121, 160, 281, 160);

  // Draw the bottom, indented 3-D rectangle.
  // Top
  XDrawLine(g_display, g_win, g_gc, 120, 180, 279, 180);
  // Left
  XDrawLine(g_display, g_win, g_gc, 120, 221, 120, 180);
  XSetForeground(g_display, g_gc, WhitePixel(g_display, screen_num));
  // Right
  XDrawLine(g_display, g_win, g_gc, 280, 221, 280, 180);
  // Bottom
  XDrawLine(g_display, g_win, g_gc, 121, 220, 281, 220);
}

static void init_x()
{
  char *display_env = getenv("DISPLAY");

  if ((g_display = XOpenDisplay(display_env)) == NULL) {
    if (display_env != NULL) {
      fprintf(stderr, "Tried to connect with %s...", display_env);
    }
    fprintf(stderr, "Can't open display.\n");
    exit(1);
  }
}

static Window create_window(int width, int height)
{
  screen_num = DefaultScreen(g_display);

  // Set window attributes, the only ones we care about for now are
  // background and border pixel.
  XSetWindowAttributes attr;
  unsigned long mask = CWBackPixel | CWBorderPixel;
  attr.background_pixel = BlackPixel(g_display, screen_num);
  attr.border_pixel = BlackPixel(g_display, screen_num);

  return XCreateWindow(g_display, RootWindow(g_display, screen_num),
      100, 200, width, height, 1, CopyFromParent /* depth */,
      CopyFromParent /* class */, CopyFromParent /* visual */,
      mask, &attr);
}

static GC create_gc()
{
  XGCValues values;

  values.background = WhitePixel(g_display, screen_num);
  values.foreground = BlackPixel(g_display, screen_num);

  GC gc = XCreateGC(g_display, g_win,
      GCForeground | GCBackground, &values);
  if (gc < 0) {
    fprintf(stderr, "Failed to create GC\n");
    exit(1);
  }

  XSetLineAttributes(g_display, gc,
      1, // line width in pixels
      LineSolid, // line style for drawing
      CapButt, // style for line's edge
      JoinBevel); // style for joined lines.

  // define the fill style for the GC, to be 'solid filling'
  XSetFillStyle(g_display, gc, FillSolid);

  return gc;
}

///////////////////////////////////////////////////////////
// StartGraphics()
//
// This function initializes Borland's graphics driver
// for the high-resolution VGA screen.
///////////////////////////////////////////////////////////
void StartGraphics(void)
{
  init_x();
  g_win = create_window(640, 480);

  XSetStandardProperties(g_display, g_win, "3D Demo", "3D Demo",
      None, NULL, 0, NULL);

  XSelectInput(g_display, g_win,
      ExposureMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask);

  g_gc = create_gc();

  XColor xcol;

  // Setup light and dark gray colors.
  xcol.red = 0x8000;
  xcol.green = 0x8000;
  xcol.blue = 0x8000;

  XAllocColor(g_display, DefaultColormap(g_display, DefaultScreen(g_display)),
      &xcol);
  dkgray = xcol.pixel;

  xcol.red = 0xC000;
  xcol.green = 0xC000;
  xcol.blue = 0xC000;

  XAllocColor(g_display, DefaultColormap(g_display, DefaultScreen(g_display)),
      &xcol);
  ltgray = xcol.pixel;

  XMapWindow(g_display, g_win);
}
