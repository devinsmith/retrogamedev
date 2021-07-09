///////////////////////////////////////////////////////////
// LIST.CPP: The list class's implementation.
///////////////////////////////////////////////////////////

#include "list.h"

///////////////////////////////////////////////////////////
// List::List()
//
// This is the list class's constructor.
///////////////////////////////////////////////////////////
List::List(void)
{
   // Initialize list head and tail pointers.
   list_h = list_t = NULL;
}

///////////////////////////////////////////////////////////
// List::~List()
//
// This is the list class's destructor.
///////////////////////////////////////////////////////////
List::~List(void)
{
   // Delete all nodes in the list.
   while (list_h)
   {
      node = list_h;
      list_h = list_h->next;
      delete node;
   }
}

///////////////////////////////////////////////////////////
// List::MakeNewNode()
//
// This function creates a new node and adds it to the
// linked list.
///////////////////////////////////////////////////////////
void List::MakeNewNode(int n1, int n2)
{
   node = new Node;
   node->x = n1;
   node->y = n2;
   if (!list_h)
      list_h = node;
   else
      list_t->next = node;
   list_t = node;
   list_t->next = NULL;
}

///////////////////////////////////////////////////////////
// List::DisplayList()
//
// This function displays the contents of each node in
// the linked list.
///////////////////////////////////////////////////////////
void List::DisplayList(void)
{
   node = list_h;
   while (node)
   {
      cout << node->x << ',' << node->y << '\n';
      node = node->next;
   }
}
