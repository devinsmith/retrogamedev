///////////////////////////////////////////////////////////
// CLIST.H: Header file for the CList class.
///////////////////////////////////////////////////////////

#ifndef _CLIST_H
#define _CLIST_H

#include <iostream.h>

// The CList class declaration.
class CList
{
   struct Node
   {
      int x, y;
      Node *next;
   };

   Node *node, *list_h, *list_t;

public:
   CList(void);
   ~CList(void);
   void MakeNewNode(int n1, int n2);
   void TransferList(CList &list2);
   void ClearList(void);
   int HasNodes(void);
   void GetNode(int &c, int &r);
   void DisplayList(void);
};

#endif
