///////////////////////////////////////////////////////////
// MOUS.H: Mouse class header file.
///////////////////////////////////////////////////////////

#ifndef __MOUSE_H
#define __MOUSE_H

#include <dos.h>

// Mouse button values.
#define LEFT 1
#define RIGHT 2
#define CENTER 3

// The mouse class declaration.
class Mouse
{
private:
  int mx, my,        // Mouse coordinates.
      got_mouse,     // Mouse init flag.
      num_buttons,   // # buttons on mouse.
      button;        // Button status.
  REGS inRegs;       // Input registers.
  REGS outRegs;      // Output registers.

public:
  Mouse(void);
  ~Mouse(void);
  int GotMouse(void) { return got_mouse; }
  void SetLimits(int minXLimit, int maxXLimit,
                 int minYLimit, int maxYLimit);
  void ShowMouse(void);
  void HideMouse(void);
  int Event(void);
  int GetButton(void) { return button; }
  void GetXY(int &x, int &y) { x = mx; y = my; }
  void ButtonUp(void);
};

extern Mouse mouse;

#endif // __MOUSE_H
