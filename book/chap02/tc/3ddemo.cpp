///////////////////////////////////////////////////////////
// 3DDEMO.CPP: Simple 3-D graphics drawing demonatration.
//             Make sure that Borland's EGAVGA.BGI
//             graphics driver is in the same directory
//             as the program.
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include <graphics.h>

// Function prototypes.
void StartGraphics(void);
void DrawGraphics(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  // Initialize VGA graphics.
  StartGraphics();

  // Draw the 3-D graphics.
  DrawGraphics();

  // Wait for a key press.
  getch();
  return 1;
}

///////////////////////////////////////////////////////////
// DrawGraphics()
///////////////////////////////////////////////////////////
void DrawGraphics(void)
{
  // Set the fill style and color.
  setfillstyle(SOLID_FILL, LIGHTGRAY);

  // Draw a gray background rectangle.
  bar(100, 100, 300, 240);

  // Draw the top, protruding 3-D rectangle.
  setcolor(WHITE);
  moveto(120, 160);
  lineto(120, 120);
  lineto(279, 120);
  setcolor(BLACK);
  moveto(120, 160);
  lineto(279, 160);
  lineto(279, 160);
  lineto(279, 120);

  // Draw the bottom, indented 3-D rectangle.
  moveto(120, 220);
  lineto(120, 180);
  lineto(279, 180);
  setcolor(WHITE);
  moveto(120, 220);
  lineto(279, 220);
  lineto(279, 180);
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
  if ((errorcode = graphresult()) != 0)
  {
    cout << "Graphics error:" << errorcode<< '\n';
    getch();
    abort();
  }
}
