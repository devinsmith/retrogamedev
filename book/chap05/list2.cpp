///////////////////////////////////////////////////////////
// LIST2.CPP: Linked list demonstration program 2.
///////////////////////////////////////////////////////////

#include <iostream.h>
#include <conio.h>
#include "list.h"

void main(void)
{
   List list;

   for (int i = 0; i < 10; ++i)
      list.MakeNewNode(i, i*10);
   list.DisplayList();
   getch();
}
