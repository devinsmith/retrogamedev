///////////////////////////////////////////////////////////
// EVENT.CPP: An event handler for the keyboard and the
//            mouse.
///////////////////////////////////////////////////////////

#include <bios.h>
#include "event.h"
#include "mous.h"

///////////////////////////////////////////////////////////
// KeyEvent()
//
// This function checks for and retrieves key events.
///////////////////////////////////////////////////////////
int KeyEvent(void)
{
  // Check for key press.
  int key = bioskey(1);

  // Get key if one is available.
  if (key) key = bioskey(0);

  return key;
}

///////////////////////////////////////////////////////////
// GetEvent()
//
// This function waits for an event. When one is received,
// the function constructs an event message.
///////////////////////////////////////////////////////////
void GetEvent(EventMsg &eventMsg)
{
  while ((!mouse.Event()) &&
      (!(eventMsg.key = KeyEvent()))) {}
  eventMsg.button = mouse.GetButton();
  if (eventMsg.button)
  {
    eventMsg.type = MBUTTON;
    mouse.GetXY(eventMsg.mx, eventMsg.my);
  }
  else
  {
    eventMsg.type = KEYBD;
    eventMsg.mx = -1;
    eventMsg.my = -1;
  }
}

