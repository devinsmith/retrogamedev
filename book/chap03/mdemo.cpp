///////////////////////////////////////////////////////////
// MDEMO.CPP: Mouse test program. Before running, make
// sure the VGA graphics driver EGAVGA.BGI is in the same
// directory as the program.
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <iostream.h>
#include <stdlib.h>
#include "mous.h"

// Function prototypes.
void StartGraphics(void);
void TestMouse(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  // Initialize the graphics screen.
  StartGraphics();

  // Run main program loop.
  TestMouse();

  // Shut down the graphics driver.
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

///////////////////////////////////////////////////////////
// TestMouse()
//
// This function checks the mouse's status and prints the
// mouse's coordinates on the screen whenever the user
// presses the left mouse button.
///////////////////////////////////////////////////////////
void TestMouse(void)
{
  int mbutton, mouseX, mouseY;
  char s[10];

  // Check that the mouse exists.
  if (!mouse.GotMouse())
  {
    cout << "Got no mouse!";
    getch();
  }
  else
  {
    // Allow the mouse to use entire VGA screen.
    mouse.SetLimits(0, getmaxx(), 0, getmaxy());

    // Display the mouse pointer.
    mouse.ShowMouse();

    // Main program loop. Keep going until the
    // user pressed the right mouse button.
    mbutton = 0;
    while (mbutton != RIGHT)
    {
      // Update the mouse data.
      mouse.Event();

      // Get the mouse button status.
      mbutton = mouse.GetButton();

      // If the left button was pressed...
      if (mbutton == LEFT)
      {
        // Get the new mouse coordinates.
        mouse.GetXY(mouseX, mouseY);

        // Convert coordinates to a string.
        sprintf(s, "%d,%d", mouseX, mouseY);

        // Hide the pointer before drawing
        // text to the screen.
        mouse.HideMouse();

        // Display the mouse coordinates.
        outtextxy(mouseX, mouseY, s);

        // Restore the mouse pointer.
        mouse.ShowMouse();

        // Wait for the mouse button to be released.
        mouse.ButtonUp();
      }
    }
  }
}
