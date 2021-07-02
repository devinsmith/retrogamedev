///////////////////////////////////////////////////////////
// MOUS.CPP: Mouse class implementation.
///////////////////////////////////////////////////////////

#include <dos.h>
#include <conio.h>
#include "mous.h"

Mouse mouse;

///////////////////////////////////////////////////////////
// Mouse::Mouse()
//
// This is the class's constructor, which initializes the
// mouse driver.
///////////////////////////////////////////////////////////
Mouse::Mouse(void)
{
  got_mouse = 0;
  if (getvect(0x33))
  {
    inRegs.x.ax = 0x0000;
    int86(0x33, &inRegs, &outRegs);
    got_mouse = outRegs.x.ax;
    num_buttons = outRegs.x.bx;
  }
}

///////////////////////////////////////////////////////////
// Mouse::~Mouse()
//
// This is the class's destructor, which sets the mouse
// driver back to its default state.
///////////////////////////////////////////////////////////
Mouse::~Mouse(void)
{
  inRegs.x.ax = 0x0000;
  int86(0x33, &inRegs, &outRegs);
}

///////////////////////////////////////////////////////////
// Mouse::SetLimits()
//
// This function sets the mouse's screen coordinate limits.
///////////////////////////////////////////////////////////
void Mouse::SetLimits(int minXLimit, int maxXLimit,
                      int minYLimit, int maxYLimit)
{
  if (!got_mouse)
    return;

  inRegs.x.ax = 0x0007;
  inRegs.x.cx = minXLimit;
  inRegs.x.dx = maxXLimit;
  int86(0x33, &inRegs, &outRegs);
  inRegs.x.ax = 0x0008;
  inRegs.x.cx = minYLimit;
  inRegs.x.dx = maxYLimit;
  int86(0x33, &inRegs, &outRegs);
}

///////////////////////////////////////////////////////////
// Mouse::ShowMouse()
//
// This function shows the mouse pointer on-screen.
///////////////////////////////////////////////////////////
void Mouse::ShowMouse(void)
{
  if (!got_mouse)
    return;

  inRegs.x.ax = 0x0001;
  int86(0x33, &inRegs, &outRegs);
}

///////////////////////////////////////////////////////////
// Mouse::HideMouse()
//
// This function hides the mouse pointer on-screen.
///////////////////////////////////////////////////////////
void Mouse::HideMouse(void)
{
  if (!got_mouse)
    return;

  inRegs.x.ax = 0x0002;
  int86(0x33, &inRegs, &outRegs);
}

///////////////////////////////////////////////////////////
// Mouse::Event()
//
// This function checks the status of the mouse's buttons
// and gets the mouse's current position.
///////////////////////////////////////////////////////////
int Mouse::Event(void)
{
  if (!got_mouse)
    return 0;

  inRegs.x.ax = 0x0003;
  int86(0x33, &inRegs, &outRegs);
  button = outRegs.x.bx;
  mx = outRegs.x.cx;
  my = outRegs.x.dx;
  if (button)
    return 1;
  else
    return 0;
}

///////////////////////////////////////////////////////////
// Mouse::ButtonUp()
//
// This function waits for the mouse button to be released.
///////////////////////////////////////////////////////////
void Mouse::ButtonUp(void)
{
  while (button) Event();
}

