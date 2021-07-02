///////////////////////////////////////////////////////////
// WINDW.H: Header file for the DOS window classes.
///////////////////////////////////////////////////////////

#ifndef _WINDW_H
#define _WINDW_H

#include <string.h>
#include "event.h"

#define TRUE      1
#define FALSE     0
#define OK        1
#define YES       2
#define NO        3
#define CANCEL    4
#define OKALT     0x1c0d
#define CANCELALT 0x011b

///////////////////////////////////////////////////////////
// WINDW
///////////////////////////////////////////////////////////
class Windw
{
private:
  int *buffer; // Pointer to screen buffer.

protected:
  int wx, wy, ww, wh; // Window coords.
  int border,         // Flag for border.
      buffered;      // Flag for buffer.
  EventMsg eventMsg;  // Event message.

public:
  Windw(int x, int y, int w, int h, int bdr, int buf);
  virtual ~Windw(void);
  virtual void DrawWindow(void);
  virtual void RunWindow(void);

private:
  void WindwError(char *s);
};

#endif // _WINDW_H
