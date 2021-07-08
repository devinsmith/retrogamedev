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

///////////////////////////////////////////////////////////
// CAPWINDW
///////////////////////////////////////////////////////////
class CapWindw : public Windw
{
protected:
  char label[61];

public:
  CapWindw(int x, int y, int w, int h,
      int bdr, int buf, char *s);
  virtual void DrawWindow(void);
  void SetCaption(char *s);

private:
  void DrawCapBar(void);
};

///////////////////////////////////////////////////////////
// CAPTWINDW
///////////////////////////////////////////////////////////
class CapTWindw : public CapWindw
{
protected:
  char *line1, *line2;
  int button;

public:
  CapTWindw(char *s1, char *s2, char *s3);
  virtual void DrawWindow(void);
  int GetButton(void) { return button; }
};

///////////////////////////////////////////////////////////
// BUTTON
///////////////////////////////////////////////////////////
class Button : public Windw
{
private:
  char label[20];
  unsigned hotkey;
  int altkey;

public:
  Button(int x, int y, char *s);
  void DrawWindow(void);
  int Clicked(EventMsg eventMsg);
  void ClickButton(void);
};

///////////////////////////////////////////////////////////
// OKWINDW
///////////////////////////////////////////////////////////
class OKWindw : public CapTWindw
{
private:
  Button *butn;

public:
  OKWindw(char *s1, char *s2, char *s3);
  virtual ~OKWindw(void);
  virtual void DrawWindow(void);
  virtual void RunWindow(void);
};

///////////////////////////////////////////////////////////
// YESNOWINDW
///////////////////////////////////////////////////////////
class YesNoWindw : public CapTWindw
{
private:
  Button *butn1, *butn2;

public:
  YesNoWindw(char *s1, char *s2, char *s3);
  virtual ~YesNoWindw(void);
  virtual void DrawWindow(void);
  virtual void RunWindow(void);
};

///////////////////////////////////////////////////////////
// YESNOCANWINDW
///////////////////////////////////////////////////////////
class YesNoCanWindw : public CapTWindw
{
private:
  Button *butn1, *butn2, *butn3;

public:
  YesNoCanWindw(char *s1, char *s2, char *s3);
  virtual ~YesNoCanWindw(void);
  virtual void DrawWindow(void);
  virtual void RunWindow(void);
};

///////////////////////////////////////////////////////////
// INPUTWINDW
///////////////////////////////////////////////////////////
class InputWindw : public CapTWindw
{
private:
  char input[81];
  Button *butn1, *butn2;

public:
  InputWindw(char *s1, char *s2, char *s3);
  virtual ~InputWindw(void);
  void GetInput(char *s) { strcpy(s, input); }
  virtual void DrawWindow(void);
  virtual void RunWindow(void);

private:
  void HandleInput(char k);
};

#endif // _WINDW_H
