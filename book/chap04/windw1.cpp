///////////////////////////////////////////////////////////
// WNDW1.CPP: Window demonstration program 1
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include "windw.h"

// Function prototype.
void StartGraphics(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  // Initialize the graphics screen.
  StartGraphics();

  // Set the background color;
  setbkcolor(BLUE);

  // Create the first window and show it.
  Windw wndw1(150, 100, 200, 200, FALSE, FALSE);
  wndw1.DrawWindow();

  // Craete the second window, show it, and run it.
  Windw *wndw2 = new Windw(200, 150, 200, 200, TRUE, TRUE);
  wndw2->DrawWindow();
  wndw2->RunWindow();

  // Delete the second window.
  delete wndw2;

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
