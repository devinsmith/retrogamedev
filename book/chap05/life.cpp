/////////////////////////////////////////////////////////
// CONWAY'S LIFE
// by Clayton Walnum
// Written with Turbo C++ 3.0
////////////////////////////////////////////////////////

#include <graphics.h>
#include <conio.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

// Use mouse, window, and list classes.
#include "mous.h"
#include "windw.h"
#include "clist.h"

// Use the event handler.
#include "event.h"

// Define global constants.
const DEAD   = 0;
const ALIVE  = 1;
const MAXCOL = 50;
const MAXROW = 28;

int mousex, mousey, // Mouse coordinates.
    repeat;         // Main program loop controller.
int generations,    // # of life generations.
    speed;          // Speed of simulation.

int world[MAXROW][MAXCOL], // Cell map.
    nbrs[MAXROW][MAXCOL];  // Neighbor count map.

// Linked lists.
CList live, die, nextlive, nextdie;

// Windows and controls for main screen.
CapWindw wnd1(0, 0, 639, 479, TRUE, FALSE, "CONWAY'S LIFE");
Windw wnd2(20, 409, 599, 50, FALSE, FALSE);
Windw wnd3(420, 419, 170, 32, FALSE, FALSE);;
Button startbut(30, 420, "^START");
Button clearbut(105, 420, "^CLEAR");
Button generatebut(180, 420, "^GENER");
Button speedbut(255, 420, "S^PEED");
Button quitbut(330, 420, "^QUIT");

// Event message structure.
EventMsg eventMsg;

// Function prototypes.
void DispatchEvent(EventMsg eventMsg);
void Life(void);
void CreateLists(void);
void ClearWorld(void);
void GetGens(int &generations);
void GetSpeed(int &speed);
void PlaceCell(EventMsg eventMsg);
void Live(void);
void Die(void);
void AddNbrs(void);
void SubNbrs(void);
void CalcLimits(int c, int r, int &xLow,
   int &xHigh, int &yLow, int &yHigh);
void UpdateGens(int g);
void Init(void);
void ReleaseNodes(void);
void DrawScreen(void);
void StartGraphics(void);
void InitMouse(void);

////////////////////////////////////////////////////////
// Main program.
////////////////////////////////////////////////////////
void main(void)
{
   // Initialize game, mouse, and screen.
   Init();
   DrawScreen();
   InitMouse();

   // Repeat event loop until Quit.
   repeat = 1;
   while (repeat)
   {
      GetEvent(eventMsg);
      DispatchEvent(eventMsg);
   }

   closegraph();
}

////////////////////////////////////////////////////////
// DispatchEvent()
//
// This function checks the current event message and
// branches to the function chosen by the user.
////////////////////////////////////////////////////////
void DispatchEvent(EventMsg eventMsg)
{
   if (startbut.Clicked(eventMsg))
      Life();
   else if (clearbut.Clicked(eventMsg))
      ClearWorld();
   else if (generatebut.Clicked(eventMsg))
      GetGens(generations);
   else if (speedbut.Clicked(eventMsg))
      GetSpeed(speed);
   else if (quitbut.Clicked(eventMsg))
   {
      YesNoWindw wndw("QUIT",
         "Are you sure you", "want to quit?");
     wndw.DrawWindow();
     wndw.RunWindow();
     if (wndw.GetButton() == YES)
        repeat = 0;
   }
  else
     PlaceCell(eventMsg);
}

////////////////////////////////////////////////////////
// Life()
//
// This function is the simulation's main loop and is
// called when the user selects the Start button.
////////////////////////////////////////////////////////
void Life(void)
{
   mouse.ButtonUp();
   CreateLists();
   for (int g=0; g<generations; ++g)
   {
      delay(speed);
      UpdateGens(g);
      Live();
      Die();
      AddNbrs();
      SubNbrs();
      nextlive.TransferList(live);
      nextdie.TransferList(die);
      if (KeyEvent() || mouse.Event())
      {
         mouse.ButtonUp();
         break;
      }
   }
}

////////////////////////////////////////////////////////
// GetGens()
//
// This function creates a dialog box with which the
// user can change the number of generations to run in
// each cycle of the Life() function.
////////////////////////////////////////////////////////
void GetGens(int &generations)
{
   InputWindw w("GENERATIONS",
      "Enter # of generations:", "(Max = 10,000)");
   char inp[81];

   w.DrawWindow();
   w.RunWindow();
   if (w.GetButton() == 1)
   {
      w.GetInput(inp);
      generations = atoi(inp);
      if (generations < 1)
         generations = 100;
      if (generations > 10000)
         generations = 10000;
      sprintf(inp, "Generation #%d", generations);
      setfillstyle(SOLID_FILL, LIGHTGRAY);
      settextjustify(LEFT_TEXT, TOP_TEXT);
      bar(435, 431, 575, 441);
      setcolor(BROWN);
      outtextxy(435, 431, inp);
   }
}

////////////////////////////////////////////////////////
// GetSpeed()
//
// This function creates a dialog box with which the
// user can change the speed of the simulation.
////////////////////////////////////////////////////////
void GetSpeed(int &speed)
{
   InputWindw w("SIMULATION SPEED",
      "Enter new speed:", "(Min=1  Max=10)");
   char inp[81];

   w.DrawWindow();
   w.RunWindow();
   if (w.GetButton() == 1)
   {
      w.GetInput(inp);
      speed = atoi(inp);
      if (speed < 1)
         speed = 10;
      if (speed > 10)
         speed = 10;
      speed = (10 - speed) * 100;
   }
}

////////////////////////////////////////////////////////
// ClearWorld()
//
// This function clears all cells from the map.
////////////////////////////////////////////////////////
void ClearWorld(void)
{
   mouse.HideMouse();
   for (int c=0; c<MAXCOL; ++c)
      for (int r=0; r<MAXROW; ++r)
         if (world[r][c] == ALIVE)
         {
	    world[r][c] = DEAD;
	    setfillstyle(SOLID_FILL, LIGHTGRAY);
	    setcolor(LIGHTGRAY);
	    fillellipse(c*12+26, r*12+61, 4, 4);
         }
   ReleaseNodes();
   mouse.ShowMouse();
}

////////////////////////////////////////////////////////
// PlaceCell()
//
// This function places a cell on the screen where the
// user clicked the map.
////////////////////////////////////////////////////////
void PlaceCell(EventMsg eventMsg)
{
   if ((eventMsg.mx > 20) && (eventMsg.mx < 620) &&
       (eventMsg.my > 56) && (eventMsg.my < 390))
   {
      mouse.HideMouse();
      int col = (eventMsg.mx - 20) / 12;
      int row = (eventMsg.my - 56) / 12;
      if (!world[row][col])
      {
         setfillstyle(SOLID_FILL, GREEN);
         setcolor(RED);
         fillellipse(col*12+26, row*12+61, 4, 4);
         world[row][col] = ALIVE;
      }
      mouse.ShowMouse();
   }
}

////////////////////////////////////////////////////////
// CreateLists()
//
// This function initializes the cell maps and linked
// lists for the Life() function.
////////////////////////////////////////////////////////
void CreateLists(void)
{
   int c, r;

   ReleaseNodes();
   for (c=0; c<MAXCOL; ++c)
      for (r=0; r<MAXROW; ++r)
      {
         nbrs[r][c] = 0;
         if (world[r][c] == ALIVE)
	   live.MakeNewNode(c, r);
      }
   AddNbrs();
   for (c=0; c<MAXCOL; ++c)
      for (r=0; r<MAXROW; ++r)
         if (((nbrs[r][c] < 2) || (nbrs[r][c] > 3))
	       && (world[r][c] == ALIVE))
	    nextdie.MakeNewNode(c, r);
   nextlive.TransferList(live);
   nextdie.TransferList(die);
}

////////////////////////////////////////////////////////
// Live()
//
// This function scans the live linked list and brings
// to life any cell that fits the requirements for life.
// Cells that come to life are placed back into the live
// list. Cells that don't meet the requirements for life
// are deleted.
////////////////////////////////////////////////////////
void Live(void)
{
   CList temp;
   int r, c;

   live.TransferList(temp);
   while(temp.HasNodes())
   {
      temp.GetNode(c, r);
      if ((world[r][c] == DEAD) &&
	  (nbrs[r][c] == 3))
      {
         world[r][c] = ALIVE;
         mouse.HideMouse();
         setcolor(RED);
         setfillstyle(SOLID_FILL, LIGHTRED);
         fillellipse(c*12+26, r*12+61, 4, 4);
         mouse.ShowMouse();
         live.MakeNewNode(c, r);
      }
   }
}

////////////////////////////////////////////////////////
// Die()
//
// This function scans the die linked list and kills
// any cell that fits the requirements for death.
// Cells that die are placed back into the die list.
// Cells that don't meet the requirements for death
// are deleted.
////////////////////////////////////////////////////////
void Die(void)
{
   CList temp;
   int c, r;

   die.TransferList(temp);
   while(temp.HasNodes())
   {
      temp.GetNode(c, r);
      if ((world[r][c] == ALIVE) &&
	  (nbrs[r][c] != 2) &&
	  (nbrs[r][c] != 3))
      {
         world[r][c] = DEAD;
         mouse.HideMouse();
         setcolor(LIGHTGRAY);
         setfillstyle(SOLID_FILL, LIGHTGRAY);
         fillellipse(c*12+26, r*12+61, 4, 4);
         mouse.ShowMouse();
         die.MakeNewNode(c, r);
      }
   }
}

////////////////////////////////////////////////////////
// AddNbrs()
//
// This function increments the neighbor count of every
// cell adjacent to a cell that has just come to life.
// Cells that might come to life in the next generation
// are added to the nextlive list, and cells that might
// die in the next generation are added to the nextdie
// list. This function leaves the live list empty.
////////////////////////////////////////////////////////
void AddNbrs(void)
{
   int xLow, xHigh, yLow, yHigh;
   int c, r;

   while (live.HasNodes())
   {
      live.GetNode(c, r);
      CalcLimits(c, r, xLow, xHigh, yLow, yHigh);
      for (int x=xLow; x<=xHigh; ++x)
      {
         for (int y=yLow; y<=yHigh; ++y)
         {
	    if ((x != c) || (y != r))
	    {
	       nbrs[y][x] += 1;
	       switch (nbrs[y][x])
	       {
	          case 1, 2: break;
	          case 3:
	             if (world[y][x] == DEAD)
		        nextlive.MakeNewNode(x, y);
	             break;
	          case 4:
	             if (world[y][x] == ALIVE)
		        nextdie.MakeNewNode(x, y);
	             break;
	          case 5, 6, 7, 8: break;
	       }
	    }
         }
      }
   }
}

////////////////////////////////////////////////////////
// SubNbrs()
//
// This function decrements the neighbor count of every
// cell adjacent to a cell that has just died. Cells
// that might die in the next generation are added to
// the nextdie list, and cells that might come to life
// in the next generation are added to the nextlive list.
// This function leaves the die list empty.
////////////////////////////////////////////////////////
void SubNbrs(void)
{
   int xLow, xHigh, yLow, yHigh;
   int c, r;

   while (die.HasNodes())
   {
      die.GetNode(c, r);
      CalcLimits(c, r, xLow, xHigh, yLow, yHigh);
      for (int x=xLow; x<=xHigh; ++x)
      {
         for (int y=yLow; y<=yHigh; ++y)
         {
	    if ((x != c) || (y != r))
	    {
	       nbrs[y][x] -= 1;
	       switch (nbrs[y][x])
	       {
	          case 0: break;
	          case 1:
	             if (world[y][x] == ALIVE)
		        nextdie.MakeNewNode(x, y);
	             break;
	          case 2: break;
	          case 3:
	             if (world[y][x] == DEAD)
		        nextlive.MakeNewNode(x, y);
	             break;
	          case 4, 5, 6, 7: break;
	       }
            }
         }
      }
   }
}

////////////////////////////////////////////////////////
// CalcLimits()
//
// This function calculates the beginning and ending
// columns and rows to be checked by the AddNbrs() and
// SubNbrs() functions.
////////////////////////////////////////////////////////
void CalcLimits(int c, int r, int &xLow, int &xHigh,
		int &yLow, int &yHigh)
{
   if (c == 0)
      xLow = 0;
   else
      xLow = c - 1;
   if (c == MAXCOL-1)
      xHigh = MAXCOL-1;
   else
      xHigh = c + 1;
   if (r == 0)
      yLow = 0;
   else
      yLow = r - 1;
  if (r == MAXROW-1)
     yHigh = MAXROW-1;
  else
     yHigh = r + 1;
}

////////////////////////////////////////////////////////
// UpdateGens()
//
// This function draws the generation count on the
// screen.
////////////////////////////////////////////////////////
void UpdateGens(int g)
{
   char s[10];

   mouse.HideMouse();
   setcolor(BROWN);
   setfillstyle(SOLID_FILL, LIGHTGRAY);
   bar(531, 431, 580, 439);
   sprintf(s, "%d", g+1);
   outtextxy(531, 431, s);
   mouse.ShowMouse();
}

////////////////////////////////////////////////////////
// Init()
//
// This function performs general program initialization,
// initializing the graphics driver, setting all cells
// in the map to their DEAD state, and setting the
// default simulation speed and number of generations.
////////////////////////////////////////////////////////
void Init(void)
{
   // Set up VGA graphics mode.
   StartGraphics();

   // Set all cells to dead.
   for (int r=0; r<MAXROW; ++r)
      for (int c=0; c<MAXCOL; ++c)
         world[r][c] = DEAD;

   // Set default values.
   generations = 100;
   speed = 1;
}

////////////////////////////////////////////////////////
// DrawScreen()
//
// This function draws the main screen.
////////////////////////////////////////////////////////
void DrawScreen(void)
{
   // Draw windows and buttons.
   wnd1.DrawWindow();
   wnd2.DrawWindow();
   wnd3.DrawWindow();
   startbut.DrawWindow();
   clearbut.DrawWindow();
   generatebut.DrawWindow();
   speedbut.DrawWindow();
   quitbut.DrawWindow();

   // Draw grid lines.
   setcolor(BLUE);
   for (int y=55; y<400; y+=12)
   {
      moveto(20, y);
      lineto(getmaxx()-20, y);
   }
   for (int x=20; x<630; x+=12 )
   {
      moveto(x, 55);
      lineto(x, 391);
   }

   // Draw generation display.
   setcolor(BROWN);
   outtextxy(435, 431, "Generation #100");
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

   errorcode = registerbgidriver(EGAVGA_driver);
   if (errorcode < 0)
   {
      cout << "Graphics not initialized: ";
      cout << errorcode << endl;
      cout << "Press any key.";
      getch();
      abort();
   }

   initgraph(&gdriver, &gmode, "");
   if ((errorcode = graphresult()) != grOk)
   {
      cout << "Graphics not initialized: ";
      cout << errorcode << endl;
      cout << "Press any key.";
      getch();
      abort();
   }
}

////////////////////////////////////////////////////////
// InitMouse()
//
// This function initializes the user's mouse.
////////////////////////////////////////////////////////
void InitMouse(void)
{
   if (!mouse.GotMouse()) {
      cout << "You have no mouse.\n";
      cout << "Press any key.";
      getch();
   }
   mouse.SetLimits(0,getmaxx(),0,getmaxy());
   mouse.ShowMouse();
}

////////////////////////////////////////////////////////
// ReleaseNodes()
//
// This function deletes all nodes from the linked
// lists.
////////////////////////////////////////////////////////
void ReleaseNodes(void)
{
   live.ClearList();
   die.ClearList();
   nextlive.ClearList();
   nextdie.ClearList();
}
