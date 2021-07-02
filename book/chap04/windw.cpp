///////////////////////////////////////////////////////////
// WINDW.CPP: Implementation of the DOS window classes.
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <alloc.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include "mous.h"
#include "windw.h"

// Values for CTRL-A through CTRL-Z
unsigned ctrlkeys[] = {
  0x1e01, 0x3002, 0x2e03, 0x2004, 0x1205, 0x2106,
  0x2207, 0x2308, 0x1709, 0x240a, 0x250b, 0x260c,
  0x320d, 0x310e, 0x180f, 0x1910, 0x1011, 0x1312,
  0x1f13, 0x1414, 0x1615, 0x2f16, 0x1117, 0x2d18,
  0x1519, 0x2c1a
};

// --------------------------------------------------------
// Implementation of the Windw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// Windw::Windw()
///////////////////////////////////////////////////////////
Windw::Windw(int x, int y, int w, int h, int brd, int buf)
{
  wx = x; wy = y; ww = w; wh = h;
  border = brd;
  buffered = buf;
  buffer = NULL;
}

///////////////////////////////////////////////////////////
// Windw::~Windw()
///////////////////////////////////////////////////////////
Windw::~Windw(void)
{
  if (buffer != NULL)
  {
    mouse.HideMouse();
    putimage(wx, wy, buffer, COPY_PUT);
    free(buffer);
    mouse.ShowMouse();
  }
}

///////////////////////////////////////////////////////////
// Windw::DrawWindow()
///////////////////////////////////////////////////////////
void Windw::DrawWindow(void)
{
  int size;

  mouse.HideMouse();

  // Save window screen area, if requested.
  if (buffered)
  {
    if ((size = imagesize(wx, wy, wx+ww, wy+wh)) < 0)
      WindwError("Image too large to store.");
    else
    {
      if ((buffer = (int *)malloc(size)) == NULL)
        WindwError("Not enough memory.");
      else
        getimage(wx, wy, wx+ww, wy+wh, buffer);
    }
  }

  // Draw basic 3-D window.
  setcolor(WHITE);
  moveto(wx+ww, wy);
  lineto(wx, wy);
  lineto(wx, wy+wh);
  moveto(wx+ww-1, wy+1);
  lineto(wx+1, wy+1);
  lineto(wx+1, wy+wh-1);
  setcolor(DARKGRAY);
  moveto(wx+1, wy+wh);
  lineto(wx+ww, wy+wh);
  lineto(wx+ww, wy);
  moveto(wx+2, wy+wh-1);
  lineto(wx+ww-1, wy+wh-1);
  lineto(wx+ww-1, wy+1);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  bar(wx+2, wy+2, wx+ww-2, wy+wh-2);

  // Draw border, if requested.
  if (border) {
    setcolor(DARKGRAY);
    moveto(wx+ww-10, wy+10);
    lineto(wx+10, wy+10);
    lineto(wx+10, wy+wh-10);
    setcolor(WHITE);
    lineto(wx+ww-10, wy+wh-10);
    lineto(wx+ww-10, wy+10);
  }
  mouse.ShowMouse();
}

///////////////////////////////////////////////////////////
// Windw::RunWindow()
///////////////////////////////////////////////////////////
void Windw::RunWindow(void)
{
  GetEvent(eventMsg);
}

///////////////////////////////////////////////////////////
// Windw::WindwError()
///////////////////////////////////////////////////////////
void Windw::WindwError(char *s)
{
  cout << "ERROR: " << s << '\n';
  cout << "Press any key";
  getch();
  abort();
}

