///////////////////////////////////////////////////////////
// WINDW.CPP: Implementation of the DOS window classes.
///////////////////////////////////////////////////////////

#include <graphics.h>
#include <alloc.h>
#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include "mous.h"
#include "windw.h"

// Values for CTRL-A through CTRL-Z
unsigned ctrlkeys[] = {
  0x1e01, 0x3002, 0x2e03, 0x2004, 0x1205, 0x2106,
  0x2207, 0x2308, 0x1709, 0x240a, 0x250b, 0x260c,
  0x320d, 0x310e, 0x180f, 0x1910, 0x1011, 0x1312,
  0x1f13, 0x1414, 0x1615, 0x2f16, 0x1117, 0x2d18,
  0x1519, 0x2c1a
};

// --------------------------------------------------------
// Implementation of the Windw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// Windw::Windw()
///////////////////////////////////////////////////////////
Windw::Windw(int x, int y, int w, int h, int brd, int buf)
{
  wx = x; wy = y; ww = w; wh = h;
  border = brd;
  buffered = buf;
  buffer = NULL;
}

///////////////////////////////////////////////////////////
// Windw::~Windw()
///////////////////////////////////////////////////////////
Windw::~Windw(void)
{
  if (buffer != NULL)
  {
    mouse.HideMouse();
    putimage(wx, wy, buffer, COPY_PUT);
    free(buffer);
    mouse.ShowMouse();
  }
}

///////////////////////////////////////////////////////////
// Windw::DrawWindow()
///////////////////////////////////////////////////////////
void Windw::DrawWindow(void)
{
  int size;

  mouse.HideMouse();

  // Save window screen area, if requested.
  if (buffered)
  {
    if ((size = imagesize(wx, wy, wx+ww, wy+wh)) < 0)
      WindwError("Image too large to store.");
    else
    {
      if ((buffer = (int *)malloc(size)) == NULL)
        WindwError("Not enough memory.");
      else
        getimage(wx, wy, wx+ww, wy+wh, buffer);
    }
  }

  // Draw basic 3-D window.
  setcolor(WHITE);
  moveto(wx+ww, wy);
  lineto(wx, wy);
  lineto(wx, wy+wh);
  moveto(wx+ww-1, wy+1);
  lineto(wx+1, wy+1);
  lineto(wx+1, wy+wh-1);
  setcolor(DARKGRAY);
  moveto(wx+1, wy+wh);
  lineto(wx+ww, wy+wh);
  lineto(wx+ww, wy);
  moveto(wx+2, wy+wh-1);
  lineto(wx+ww-1, wy+wh-1);
  lineto(wx+ww-1, wy+1);
  setfillstyle(SOLID_FILL, LIGHTGRAY);
  bar(wx+2, wy+2, wx+ww-2, wy+wh-2);

  // Draw border, if requested.
  if (border) {
    setcolor(DARKGRAY);
    moveto(wx+ww-10, wy+10);
    lineto(wx+10, wy+10);
    lineto(wx+10, wy+wh-10);
    setcolor(WHITE);
    lineto(wx+ww-10, wy+wh-10);
    lineto(wx+ww-10, wy+10);
  }
  mouse.ShowMouse();
}

///////////////////////////////////////////////////////////
// Windw::RunWindow()
///////////////////////////////////////////////////////////
void Windw::RunWindow(void)
{
  GetEvent(eventMsg);
}

///////////////////////////////////////////////////////////
// Windw::WindwError()
///////////////////////////////////////////////////////////
void Windw::WindwError(char *s)
{
  cout << "ERROR: " << s << '\n';
  cout << "Press any key";
  getch();
  abort();
}

// --------------------------------------------------------
// Implementation of the CapWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// CapWindw::CapWindw()
///////////////////////////////////////////////////////////
CapWindw::CapWindw(int x, int y, int w, int h,
    int brd, int buf, char *s) :
  Windw(x, y, w, h, brd, buf)
{
  strcpy(label, s);
}

///////////////////////////////////////////////////////////
// CapWindw::DrawWindow()
///////////////////////////////////////////////////////////
void CapWindw::DrawWindow(void)
{
  // Draw basic window.
  Windw::DrawWindow();

  // Draw caption bar.
  DrawCapBar();
}

///////////////////////////////////////////////////////////
// CapWindw::SetCaption()
///////////////////////////////////////////////////////////
void CapWindw::SetCaption(char *s)
{
  strcpy(label, s);
  DrawCapBar();
}

///////////////////////////////////////////////////////////
// CapWindw::DrawCapBar()
///////////////////////////////////////////////////////////
void CapWindw::DrawCapBar(void)
{
  mouse.HideMouse();
  setcolor(WHITE);
  moveto(wx+20, wy+40);
  lineto(wx+20, wy+20);
  lineto(wx+ww-20, wy+20);
  setcolor(BLACK);
  lineto(wx+ww-20, wy+40);
  lineto(wx+20, wy+40);
  setfillstyle(SOLID_FILL, DARKGRAY);
  bar(wx+21, wy+21, wx+ww-21, wy+39);
  setcolor(WHITE);
  int x = (wx+ww/2) - (strlen(label)*4);
  outtextxy(x, wy+27, label);
  mouse.ShowMouse();
}

// --------------------------------------------------------
// Implementation of the CapTWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// CapTWindw::CapTWindw()
///////////////////////////////////////////////////////////
CapTWindw::CapTWindw(char *s1, char *s2, char *s3) :
  CapWindw(0, 0, 0, 150, FALSE, TRUE, s1)
{
  // Calculate which string is the longest and
  // use that width to calculate the window's width.
  int w = strlen(s1) * 8 + 60;
  if (strlen(s2) > strlen(s3))
    ww = strlen(s2) * 8 + 60;
  else
    ww = strlen(s3) * 8 + 60;
  if (w > ww) ww = w;

  // Enforce a minimum width.
  if (ww < 230) ww = 230;

  // Calculate the window's x,y coordinates.
  wx = 320 - ww/2;
  wy = 164;

  // Set the window's text.
  line1 = s2;
  line2 = s3;
}

///////////////////////////////////////////////////////////
// CapTWindw::DrawWindow()
///////////////////////////////////////////////////////////
void CapTWindw::DrawWindow(void)
{
  // Draw the captioned window.
  CapWindw::DrawWindow();

  // Position and draw window body text.
  mouse.HideMouse();
  int x = (wx+ww/2) - (strlen(line1)*8)/2;
  setcolor(BLACK);
  if (strlen(line2) == 0)
    outtextxy(x, wy+68, line1);
  else
  {
    outtextxy(x, wy+56, line1);
    x = (wx+ww/2) - (strlen(line2)*8)/2;
    outtextxy(x, wy+71, line2);
  }
  mouse.ShowMouse();
}

// --------------------------------------------------------
// Implementation of the OKWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// OKWindw::OKWindw()
///////////////////////////////////////////////////////////
OKWindw::OKWindw(char *s1, char *s2, char *s3) :
  CapTWindw(s1, s2, s3)
{
  butn = NULL;
}

///////////////////////////////////////////////////////////
// OKWindw::~OKWindw()
///////////////////////////////////////////////////////////
OKWindw::~OKWindw(void)
{
  if (butn != NULL) delete butn;
}

///////////////////////////////////////////////////////////
// OKWindw::DrawWindow()
///////////////////////////////////////////////////////////
void OKWindw::DrawWindow(void)
{
  CapTWindw::DrawWindow();
  butn = new Button(wx+ww/2-32, wy+wh-42, "^OK");
  butn->DrawWindow();
}

///////////////////////////////////////////////////////////
// OKWindw::RunWindow()
///////////////////////////////////////////////////////////
void OKWindw::RunWindow(void)
{
  button = 0;
  // Loop until a button is chosen.
  while (!button)
  {
    GetEvent(eventMsg);

    // Check for mouse click on button.
    if (butn->Clicked(eventMsg))
      button = OK;

    // Check for a keyboard event.
    else if (eventMsg.type == KEYBD)
    {
      // Convert character code to ASCII,
      // and check for Esc key.
      char k = eventMsg.key & 0x00ff;
      if (k == ESC)
        button = CANCEL;
    }
  }
}

// --------------------------------------------------------
// Implementation of the YesNoWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// YesNoWindw::YesNoWindw()
///////////////////////////////////////////////////////////
YesNoWindw::YesNoWindw(char *s1, char *s2, char *s3) :
  CapTWindw(s1, s2, s3)
{
  butn1 = butn2 = NULL;
}

///////////////////////////////////////////////////////////
// YesNoWindw::~YesNoWindw()
///////////////////////////////////////////////////////////
YesNoWindw::~YesNoWindw(void)
{
  if (butn1 != NULL) delete butn1;
  if (butn2 != NULL) delete butn2;
}

///////////////////////////////////////////////////////////
// YesNoWindw::DrawWindow()
///////////////////////////////////////////////////////////
void YesNoWindw::DrawWindow(void)
{
  CapTWindw::DrawWindow();
  butn1 = new Button(wx+ww/2-70, wy+108, "^YES");
  butn1->DrawWindow();
  butn2 = new Button(wx+ww/2+6, wy+108, "^NO");
  butn2->DrawWindow();
}

///////////////////////////////////////////////////////////
// YesNoWindw::RunWindow()
///////////////////////////////////////////////////////////
void YesNoWindw::RunWindow(void)
{
  button = 0;
  // Loop until a button is chosen.
  while (!button)
  {
    GetEvent(eventMsg);

    // Check for mouse click on button.
    if (butn1->Clicked(eventMsg))
      button = YES;
    else if (butn2->Clicked(eventMsg))
      button = NO;

    // Check for a keyboard event.
    else if (eventMsg.type == KEYBD)
    {
      // Convert character code to ASCII,
      // and check for Esc key.
      char k = eventMsg.key & 0x00ff;
      if (k == ESC)
        button = CANCEL;
    }
  }
}

// --------------------------------------------------------
// Implementation of the YesNoCanWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// YesNoCanWindw::YesNoCanWindw()
///////////////////////////////////////////////////////////
YesNoCanWindw::YesNoCanWindw(char *s1, char *s2, char *s3) :
  CapTWindw(s1, s2, s3)
{
  butn1 = butn2 = butn3 = NULL;
}

///////////////////////////////////////////////////////////
// YesNoCanWindw::~YesNoCanWindw()
///////////////////////////////////////////////////////////
YesNoCanWindw::~YesNoCanWindw(void)
{
  if (butn1 != NULL) delete butn1;
  if (butn2 != NULL) delete butn2;
  if (butn3 != NULL) delete butn3;
}

///////////////////////////////////////////////////////////
// YesNoCanWindw::DrawWindow()
///////////////////////////////////////////////////////////
void YesNoCanWindw::DrawWindow(void)
{
  CapTWindw::DrawWindow();
  butn1 = new Button(wx+ww/2-105, wy+wh-42, "^YES");
  butn1->DrawWindow();
  butn2 = new Button(wx+ww/2-32, wy+wh-42, "^NO");
  butn2->DrawWindow();
  butn3 = new Button(wx+ww/2+41, wy+wh-42, "^CANCEL");
  butn3->DrawWindow();
}

///////////////////////////////////////////////////////////
// YesNoCanWindw::RunWindow()
///////////////////////////////////////////////////////////
void YesNoCanWindw::RunWindow(void)
{
  button = 0;
  // Loop until a button is chosen.
  while (!button)
  {
    GetEvent(eventMsg);

    // Check for mouse click on button.
    if (butn1->Clicked(eventMsg))
      button = YES;
    else if (butn2->Clicked(eventMsg))
      button = NO;
    else if (butn3->Clicked(eventMsg))
      button = CANCEL;

    // Check for a keyboard event.
    else if (eventMsg.type == KEYBD)
    {
      // Convert character code to ASCII,
      // and check for Esc key.
      char k = eventMsg.key & 0x00ff;
      if (k == ESC)
        button = CANCEL;
    }
  }
}

// --------------------------------------------------------
// Implementation of the InputWindw class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// InputWindw::InputWindw()
///////////////////////////////////////////////////////////
InputWindw::InputWindw(char *s1, char *s2, char *s3) :
  CapTWindw(s1, s2, s3)
{
  input[0] = 0;
  butn1 = butn2 = NULL;
}

///////////////////////////////////////////////////////////
// InputWindw::~InputWindw()
///////////////////////////////////////////////////////////
InputWindw::~InputWindw(void)
{
  if (butn1 != NULL) delete butn1;
  if (butn2 != NULL) delete butn2;
}

///////////////////////////////////////////////////////////
// InputWindw::DrawWindow()
///////////////////////////////////////////////////////////
void InputWindw::DrawWindow(void)
{
  CapTWindw::DrawWindow();
  butn1 = new Button(wx+ww/2-70, wy+108, "^OK");
  butn1->DrawWindow();
  butn2 = new Button(wx+ww/2+6, wy+108, "^CANCEL");
  butn2->DrawWindow();
  mouse.HideMouse();
  setfillstyle(SOLID_FILL, BLACK);
  bar(wx+15, wy+85, wx+ww-15, wy+99);
  mouse.ShowMouse();
}

///////////////////////////////////////////////////////////
// InputWindw::RunWindow()
///////////////////////////////////////////////////////////
void InputWindw::RunWindow(void)
{
  button = 0;
  // Loop until a button is chosen.
  while (!button)
  {
    GetEvent(eventMsg);

    // Check for mouse click on button.
    if (butn1->Clicked(eventMsg))
      button = OK;
    else if (butn2->Clicked(eventMsg))
      button = CANCEL;

    // Check for a keyboard event.
    else if (eventMsg.type == KEYBD)
    {
      // Convert character code to ASCII,
      // and check for Esc key.
      char k = eventMsg.key & 0x00ff;
      HandleInput(k);
    }
  }
}

///////////////////////////////////////////////////////////
// InputWindw::HandleInput()
///////////////////////////////////////////////////////////
void InputWindw::HandleInput(char k)
{
  int l = strlen(input);
  int w = (ww - 30)/8;
  settextjustify(LEFT_TEXT, TOP_TEXT);

  // Check that an appropriate key was pressed
  // and that the string can hold another character.
  if ((k>31) && (k<127) && (l<80))
  {
    // Add character to string.
    input[l+1] = 0; input[l] = k;

    // Draw the portion of the string that will
    // fit into the text-entry field.
    setcolor(WHITE);
    if (l < w)
      outtextxy(wx+15, wy+88, input);
    else
    {
      int i = l - w + 1;
      setfillstyle(SOLID_FILL, BLACK);
      bar(wx+15, wy+85, wx+ww-15, wy+99);
      outtextxy(wx+15, wy+88, &input[i]);
    }
  }
  // Check for a Backspace character and that
  // the string has a character to delete.
  else if ((k==BACKSP) && (l>0))
  {
    // Delete the last character.
    l -= 1;
    input[l] = 0;

    // Draw the portion of the string that
    // will fit into the text-entry field.
    setfillstyle(SOLID_FILL, BLACK);
    bar(wx+15, wy+85, wx+ww-15, wy+99);
    setcolor(WHITE);
    if (l < w)
      outtextxy(wx+15, wy+88, input);
    else
    {
      int i = l - w;
      outtextxy(wx+15, wy+88, &input[i]);
    }
  }
}

// --------------------------------------------------------
// Implementation of the Button class
// --------------------------------------------------------

///////////////////////////////////////////////////////////
// Button::Button()
///////////////////////////////////////////////////////////
Button::Button(int x, int y, char *s) :
  Windw(x, y, 64, 32, FALSE, FALSE)
{
  strcpy(label, s);
  altkey = 0;
  hotkey = 0;
}

///////////////////////////////////////////////////////////
// Button::DrawWindow()
///////////////////////////////////////////////////////////
void Button::DrawWindow(void)
{
  int pos = -1;
  char tlabel[20];

  Windw::DrawWindow();
  mouse.HideMouse();

  // Find and remove the ^ character and
  // set the appropriate hot key.
  strcpy(tlabel, label);
  for (int i = 0; i < strlen(tlabel); ++i)
  {
    if (tlabel[i] == '^')
    {
      pos = i;
      hotkey = ctrlkeys[tlabel[i+1]-65];
      for (int j = i; j < strlen(tlabel); ++j)
      {
        tlabel[j] = tlabel[j+1];
      }
    }
  }

  if (strcmp(tlabel, "OK") == 0)
    altkey = OKALT;
  else if (strcmp(tlabel, "CANCEL") == 0)
    altkey = CANCELALT;

  // Center and draw text on button.
  int x = (wx+ww/2) - (strlen(tlabel)*4);
  setcolor(BLACK);
  outtextxy(x, wy+12, tlabel);

  // Underline the hot-key character.
  if (pos >= 0)
    line(x+pos*8, wy+20, x+pos*8+6, wy+20);

  mouse.ShowMouse();
}

///////////////////////////////////////////////////////////
// Button::Clicked()
///////////////////////////////////////////////////////////
int Button::Clicked(EventMsg eventMsg)
{
  int click = FALSE;

  // Check whether the user selected the
  // button with the mouse.
  if ((eventMsg.type == MBUTTON) &&
      (eventMsg.mx>wx) && (eventMsg.mx<wx+ww) &&
      (eventMsg.my>wy) && (eventMsg.my<wy+wh))
  {
    ClickButton();
    click = TRUE;
  }

  // Check whether the user selected the
  // button from the keyboard.
  else if (eventMsg.type == KEYBD)
  {
    if ((eventMsg.key == hotkey) ||
        (eventMsg.key == altkey))
    {
      ClickButton();
      click = TRUE;
    }
  }

  return click;
}

///////////////////////////////////////////////////////////
// Button::ClickButton()
///////////////////////////////////////////////////////////
void Button::ClickButton(void)
{
  int *buff;

  mouse.HideMouse();

  // Shift the image on the button down and right
  // to simulate button movement.
  int size = imagesize(wx+2, wy+2, wx+ww-2, wy+wh-2);
  buff = (int *)malloc(size);
  if (buff)
  {
    getimage(wx+2, wy+2, wx+ww-2, wy+wh-2, buff);
    putimage(wx+3, wy+3, buff, COPY_PUT);
    free(buff);
  }

  // Draw the button's borders so the
  // button appears to be depressed.
  setcolor(DARKGRAY);
  moveto(wx+ww, wy);
  lineto(wx, wy); lineto(wx, wy+wh);
  moveto(wx+ww-1, wy+1);
  lineto(wx+1, wy+1); lineto(wx+1, wy+wh-1);
  setcolor(WHITE);
  moveto(wx+1, wy+wh);
  lineto(wx+ww, wy+wh); lineto(wx+ww, wy);
  moveto(wx+2, wy+wh-1);
  lineto(wx+ww-1, wy+wh-1);
  lineto(wx+ww-1, wy+1);

  // Make button beep.
  sound(2000);
  delay(100);
  nosound();

  // Redraw button in unselected form.
  DrawWindow();

  mouse.ShowMouse();
}

