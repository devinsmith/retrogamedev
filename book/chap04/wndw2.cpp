///////////////////////////////////////////////////////////
// WNDW2.CPP: Window demonstration program 2.
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include "mous.h"
#include "windw.h"
#include "event.h"

// Function prototype.
void StartGraphics(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  EventMsg eventMsg;

  // Initialize the graphics screen.
  StartGraphics();

  // Set the background color;
  // Draw a blue background.
  setfillstyle(SOLID_FILL, BLUE);
  bar(0, 0, getmaxx(), getmaxy());

  // Set up the mouse.
  mouse.SetLimits(0, getmaxx(), 0, getmaxy());
  mouse.ShowMouse();

  // Create and show two buttons.
  Button wndw1(200, 200, "^OK");
  Button wndw2(280, 200, "^CANCEL");
  wndw1.DrawWindow();
  wndw2.DrawWindow();

  // Loop until the user chooses a button.
  int button = 0;
  while (!button)
  {
    // Wait for an event.
    GetEvent(eventMsg);

    // Check for a button click.
    if (wndw1.Clicked(eventMsg))
      button = OK;
    else if (wndw2.Clicked(eventMsg))
      button = CANCEL;
  }

  // Hide the mouse before writing to the screen.
  mouse.HideMouse();

  // Display the button that was clicked.
  if (button == OK)
    outtextxy(200, 300, "You clicked OK.");
  else
    outtextxy(200, 300, "You clicked CANCEL.");

  // Wait for a key press, and then close graphics.
  getch();
  closegraph();

  return 1;
}

///////////////////////////////////////////////////////////
// StartGraphics()
//
// This function initializes Borland's graphics driver
// for the high-resolution VGA screen.
///////////////////////////////////////////////////////////
void StartGraphics(void)
{
  int gdriver = VGA, gmode = VGAHI, errorcode;

  initgraph(&gdriver, &gmode, "");
  if ((errorcode = graphresult()) != grOk)
  {
    cout << "Graphics error:" << errorcode<< '\n';
    getch();
    abort();
  }
}
