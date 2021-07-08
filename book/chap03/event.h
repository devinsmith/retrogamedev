///////////////////////////////////////////////////////////
// EVENT.H: Event handler header file.
///////////////////////////////////////////////////////////

#ifndef __EVENT_H
#define __EVENT_H

#define MBUTTON 1
#define KEYBD   2

#define CR     13
#define ESC    27
#define BACKSP 8

// Event message structure.
struct EventMsg
{
  int type,     // Event type.
      mx, my,   // Mouse coordinates.
      button;   // Mouse button pressed.
  unsigned key; // Key pressed.
};

// Function prototypes.
int KeyEvent(void);
void GetEvent(EventMsg &eventMsg);

#endif // __EVENT_H
