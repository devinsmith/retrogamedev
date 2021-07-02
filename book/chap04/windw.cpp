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

// --------------------------------------------------------
// Implementation of the CapWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// CapWindw::CapWindw()
///////////////////////////////////////////////////////////
CapWindw::CapWindw(int x, int y, int w, int h,
    int brd, int buf, char *s) :
  Windw(x, y, w, h, brd, buf)
{
  strcpy(label, s);
}

///////////////////////////////////////////////////////////
// CapWindw::DrawWindow()
///////////////////////////////////////////////////////////
void CapWindw::DrawWindow(void)
{
  // Draw basic window.
  Windw::DrawWindow();

  // Draw caption bar.
  DrawCapBar();
}

///////////////////////////////////////////////////////////
// CapWindw::SetCaption()
///////////////////////////////////////////////////////////
void CapWindw::SetCaption(char *s)
{
  strcpy(label, s);
  DrawCapBar();
}

///////////////////////////////////////////////////////////
// CapWindw::DrawCapBar()
///////////////////////////////////////////////////////////
void CapWindw::DrawCapBar(void)
{
  mouse.HideMouse();
  setcolor(WHITE);
  moveto(wx+20, wy+40);
  lineto(wx+20, wy+20);
  lineto(wx+ww-20, wy+20);
  setcolor(BLACK);
  lineto(wx+ww-20, wy+40);
  lineto(wx+20, wy+40);
  setfillstyle(SOLID_FILL, DARKGRAY);
  bar(wx+21, wy+21, wx+ww-21, wy+39);
  setcolor(WHITE);
  int x = (wx+ww/2) - (strlen(label)*4);
  outtextxy(x, wy+27, label);
  mouse.ShowMouse();
}

// --------------------------------------------------------
// Implementation of the CapTWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// CapTWindw::CapTWindw()
///////////////////////////////////////////////////////////
CapTWindw::CapTWindw(char *s1, char *s2, char *s3) :
  CapWindw(0, 0, 0, 150, FALSE, TRUE, s1)
{
  // Calculate which string is the longest and
  // use that width to calculate the window's width.
  int w = strlen(s1) * 8 + 60;
  if (strlen(s2) > strlen(s3))
    ww = strlen(s2) * 8 + 60;
  else
    ww = strlen(s3) * 8 + 60;
  if (w > ww) ww = w;

  // Enforce a minimum width.
  if (ww < 230) ww = 230;

  // Calculate the window's x,y coordinates.
  wx = 320 - ww/2;
  wy = 164;

  // Set the window's text.
  line1 = s2;
  line2 = s3;
}

///////////////////////////////////////////////////////////
// CapTWindw::DrawWindow()
///////////////////////////////////////////////////////////
void CapTWindw::DrawWindow(void)
{
  // Draw the captioned window.
  CapWindw::DrawWindow();

  // Position and draw window body text.
  mouse.HideMouse();
  int x = (wx+ww/2) - (strlen(line1)*8)/2;
  setcolor(BLACK);
  if (strlen(line2) == 0)
    outtextxy(x, wy+68, line1);
  else
  {
    outtextxy(x, wy+56, line1);
    x = (wx+ww/2) - (strlen(line2)*8)/2;
    outtextxy(x, wy+71, line2);
  }
  mouse.ShowMouse();
}
