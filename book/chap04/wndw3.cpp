///////////////////////////////////////////////////////////
// WNDW3.CPP: Window demonstration program 3.
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include "mous.h"
#include "windw.h"

// Function prototype.
void StartGraphics(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
   char s[81];

   // Initialize the graphics screen.
   StartGraphics();

   // Draw a blue background.
   setfillstyle(SOLID_FILL, BLUE);
   bar(0, 0, getmaxx(), getmaxy());

   // Set up the mouse.
   mouse.SetLimits(0,getmaxx(),0,getmaxy());
   mouse.ShowMouse();

   // Create, display, and run the input window.
   InputWindw *wndw1 = new InputWindw("INPUT WINDOW",
      "Enter a text string:", "");
   wndw1->DrawWindow();
   wndw1->RunWindow();

   // If the user clicked the OK button...
   if (wndw1->GetButton() == OK)
   {
      // Get the user's input from the input window.
      wndw1->GetInput(s);

      // Hide the mouse, and show the input.
      mouse.HideMouse();
      outtextxy(0, 350, s);

      // Wait for a key press.
      getch();
   }
   // Delete the input window, and close graphics.
   delete wndw1;
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
   if ( (errorcode = graphresult()) != grOk)
   {
      cout << "Graphics error: " << errorcode << '\n';
      getch();
      abort();
   }
}
