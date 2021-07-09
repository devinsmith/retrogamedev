///////////////////////////////////////////////////////////
// LIST.H: List class header file.
///////////////////////////////////////////////////////////

#ifndef __LIST_H
#define __LIST_H

#include <iostream.h>

// The list class's declaration.
class List
{
   struct Node
   {
      int x, y;
      Node *next;
   };

   Node *node, *list_h, *list_t;

public:
   List(void);
   ~List(void);
   void MakeNewNode(int n1, int n2);
   void DisplayList(void);
};

#endif
