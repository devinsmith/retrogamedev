///////////////////////////////////////////////////////////
// PCX.H: Header file for the PCX class.
///////////////////////////////////////////////////////////

#ifndef __PCX_H
#define __PCX_H

#include <stdio.h>
#include <dos.h>
#include <string.h>

// PCX header structure type.
struct PCXHeader
{
   char pcxID;
   char version;
   char rleEncoding;
   char bitsPerPixel;
   int x1, y1;
   int x2, y2;
   int hRes;
   int vRes;
   unsigned char palette[48];
   char reserved;
   char colorPlanes;
   int bytesPerLine;
   int paletteType;
   char unused[58];
};

// PCX palette type.
typedef unsigned char PCXPalette[48];

// PCX class declaration.
class PCX
{
protected:
   PCXHeader pcxHeader;
   PCXPalette pcxPalette;
   char pcxFileName[80];
   FILE *pcxFile;
   char *screenLinePointers[480];

public:
   PCX(char *fileName);
   ~PCX();
   int OpenPCXFile(void);
   void ShowPCX(void);
   void GetPCXHeader(PCXHeader &header);
   void GetPCXPalette(PCXPalette &palette);

protected:
   void ReadPCXLine(char *buffer);
   void PCX::ShowBitPlane(int n, int line, char *buffer);
   void SetPalette();
};

#endif
