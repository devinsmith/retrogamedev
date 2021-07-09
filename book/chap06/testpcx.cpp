///////////////////////////////////////////////////////////
// TESTPCX.CPP: Test program for the PCX class.
///////////////////////////////////////////////////////////

#include <stdlib.h>
#include <conio.h>
#include <iostream.h>
#include <graphics.h>
#include "pcx.h"

// Function prototype.
void StartGraphics(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
   PCXHeader header;
   PCXPalette palette;
   char fileName[81];

   // Get PCX file name.
   clrscr();
   cout << "Enter PCX filename: ";
   cin >> fileName;
   cout << endl;

   // Construct a PCX object.
   PCX pcx(fileName);

   // Open the PCX file.
   int errorCode = pcx.OpenPCXFile();
   if (errorCode != 0)
   {
      switch (errorCode)
      {
         case 1: cout << "Couldn't open PCX file" << endl;
                 break;
         case 2: cout << "Couldn't read PCX header" << endl;
                 break;
         case 3: cout << "Not a PCX file" << endl;
                 break;
         case 4: cout << "Not a 640x480 PCX picture" << endl;
      }
      exit(1);
   }

   // Get a copy of the PCX header.
   pcx.GetPCXHeader(header);

   // Get a copy of the palette.
   pcx.GetPCXPalette(palette);

   // Show the PCX picture.
   StartGraphics();
   pcx.ShowPCX();

   // Wait for a keypress and then exit.
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
