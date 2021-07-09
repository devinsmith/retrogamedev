///////////////////////////////////////////////////////////
// PCX.CPP: Implementation file for the PCX class. This
//          class works only with 16-color, 640x480
//          PCX picture files!
///////////////////////////////////////////////////////////

#include <graphics.h>
#include "pcx.h"

// Default VGA palette.
char defaultPalette[48] =
{
   0x00,0x00,0x0E,0x00,0x52,0x07,0x2C,0x00,
   0x0E,0x00,0x00,0x00,0xF8,0x01,0x2C,0x00,
   0x85,0x0F,0x42,0x00,0x21,0x00,0x00,0x00,
   0x00,0x00,0x6A,0x24,0x9B,0x49,0xA1,0x5E,
   0x90,0x5E,0x18,0x5E,0x84,0x14,0xD9,0x95,
   0xA0,0x14,0x12,0x00,0x06,0x00,0x68,0x1F
};

///////////////////////////////////////////////////////////
// PCX::PCX()
//
// This is the PCX class's constructor, which initializes
// a PCX object.
///////////////////////////////////////////////////////////
PCX::PCX(char *fileName)
{
   // Set addresses of screen lines.
   for (int x=0; x<480; ++x)
      screenLinePointers[x] = (char *) MK_FP(0xa000, x*80);

   // Store the picture's file name.
   strcpy(pcxFileName, fileName);

   // Initialize the FILE structure pointer.
   pcxFile = NULL;
}

///////////////////////////////////////////////////////////
// PCX::~PCX()
//
// The is the PCX class's destructor, which performs
// clean-up for a PCX object before the object is
// destroyed.
///////////////////////////////////////////////////////////
PCX::~PCX()
{
   // Close the PCX picture file.
   if (pcxFile)
      fclose(pcxFile);
}

///////////////////////////////////////////////////////////
// PCX::OpenPCXFile()
//
// This function opens a PCX picture file and reads the
// picture's header and palette. A non-zero return value
// indicates an error.
///////////////////////////////////////////////////////////
int PCX::OpenPCXFile(void)
{
   // If the file is already open, return.
   if (pcxFile)
      return 0;

   // Open the file.
   pcxFile = fopen(pcxFileName, "rb");
   if (pcxFile == NULL)
      return 1;

   // Read the picture's header.
   int bytesRead =
      fread((char *) &pcxHeader, 1, sizeof(pcxHeader), pcxFile);
   if (bytesRead != sizeof(pcxHeader))
      return 2;

   // Make sure this is a PCX file.
   if (pcxHeader.pcxID != 10)
      return 3;

   // Make sure this file is 640x480.
   if ((pcxHeader.x2 != 639) || (pcxHeader.y2 != 479))
      return 4;

   // Store the picture's palette.
   if (pcxHeader.version == 3)
      memcpy(pcxPalette, defaultPalette, 48);
   else
      memcpy(pcxPalette, pcxHeader.palette, 48);

   return 0;
}

///////////////////////////////////////////////////////////
// PCX::GetPCXHeader()
//
// This function returns a copy of the picture's header.
///////////////////////////////////////////////////////////
void PCX::GetPCXHeader(PCXHeader &header)
{
   // If the file is open, copy the header...
   if (pcxFile)
      memcpy(&header, &pcxHeader, sizeof(header));

   // ...else return all zeroes.
   else
      memset(&header, 0, sizeof(header));
}

///////////////////////////////////////////////////////////
// PCX::GetPCXPalette()
//
// This function returns a copy of the picture's palette.
///////////////////////////////////////////////////////////
void PCX::GetPCXPalette(PCXPalette &palette)
{
   // If the file is open, copy the palette...
   if (pcxFile)
      memcpy(&palette, &pcxPalette, sizeof(palette));

   // ...else send back all zeroes.
   else
      memset(&palette, 0, sizeof(palette));
}

///////////////////////////////////////////////////////////
// PCX::ShowPCX()
//
// This function displays the PCX picture file. Your
// program must set the correct video mode before calling
// this function!
///////////////////////////////////////////////////////////
void PCX::ShowPCX(void)
{
   // If the file is not open, do nothing.
   if (pcxFile == NULL)
      return;

   // Seek to the beginning of the picture data.
   fseek(pcxFile, (unsigned long) sizeof(pcxHeader), SEEK_SET);

   // Set the VGA palette.
   SetPalette();

   // Create PCX line buffer.
   char *buffer = new char[320];

   // Read and display 480 lines with four bit planes each.
   for (int line=0; line<480; ++line)
   {
      // Decode one PCX line.
      ReadPCXLine(buffer);

      // Output four bit planes.
      ShowBitPlane(1, line, buffer);
      ShowBitPlane(2, line, &buffer[80]);
      ShowBitPlane(4, line, &buffer[160]);
      ShowBitPlane(8, line, &buffer[240]);
   }

   outp(0x3c4, 2);
   outp(0x3c5, 15);
   delete buffer;
}

///////////////////////////////////////////////////////////
// PCX::ReadPCXLine()
//
// This function decodes a line of PCX data and transfers
// the data to a buffer.
///////////////////////////////////////////////////////////
void PCX::ReadPCXLine(char *buffer)
{
   int byteCount, data, runCount, x;

   byteCount = 0;
   while (byteCount < 320)
   {
      // Get a byte of data.
      data = fgetc(pcxFile);

      // If this is a run-count byte...
      if (data > 192)
      {
         // Calculate the run count.
         runCount = data & 0x3f;

         // Get the data byte.
	 data = fgetc(pcxFile);

         // Duplicate the data byte runCount times.
	 for (x=0; x<runCount; ++x)
            buffer[byteCount++] = data;
      }
      // ...else if it's a data byte,
      // just write it to the buffer.
      else buffer[byteCount++] = data;
   }
}

///////////////////////////////////////////////////////////
// PCX::ShowBitPlane()
//
// This function writes data from the buffer to the
// appropriate bit plane.
///////////////////////////////////////////////////////////
void PCX::ShowBitPlane(int n, int line, char *buffer)
{
   outp(0x3c4, 2);
   outp(0x3c5, n);
   for (int byte=0; byte<80; ++byte)
      screenLinePointers[line][byte] = buffer[byte];
}

///////////////////////////////////////////////////////////
// PCX::SetPalette()
//
// This function sets the VGA palette to the picture's
// palette.
///////////////////////////////////////////////////////////
void PCX::SetPalette()
{
   // Map EGA registers to first 16 DAC registers.
   for (int x=0; x<15; ++x)
      setpalette(x, x);

   // Store the palette in the DAC registers.
   for (x=0; x<15; ++x)
      setrgbpalette(x, pcxPalette[x*3]>>2,
         pcxPalette[x*3+1]>>2, pcxPalette[x*3+2]>>2);
}
