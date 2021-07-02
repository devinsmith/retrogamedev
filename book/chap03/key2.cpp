///////////////////////////////////////////////////////////
// KEY2.CPP: Test Program for KeyEvent().
///////////////////////////////////////////////////////////

#include <iostream.h>
#include <bios.h>

#define CTRL_C 0x2e03

// Function prototype.
int KeyEvent(void);

///////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////
int main(void)
{
  int k = 0;

  while ((k = KeyEvent()) != CTRL_C)
  {
    // If the user pressed a key...
    if (k)
    {
      cout << "Key value: " << k << ". ";

      // Mask out the key's scan code.
      k &= 0x00ff;

      // Show the key.
      if (!k)
        cout << "Not an ASCII character.\n";
      else
      {
        cout << "The character is: ";
        cout << (char) k << '\n';
      }
    }
  }

  return 1;
}

///////////////////////////////////////////////////////////
// KeyEvent()
//
// This function gathers presses "on the fly."
///////////////////////////////////////////////////////////
int KeyEvent(void)
{
  // Check for key press.
  int key = bioskey(1);

  // Get key if one is available.
  if (key) key = bioskey(0);
  return key;
}
