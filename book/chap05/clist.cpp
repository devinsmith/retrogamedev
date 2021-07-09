///////////////////////////////////////////////////////////
// CLIST.CPP: Implementation for the CList class.
///////////////////////////////////////////////////////////

#include "clist.h"

///////////////////////////////////////////////////////////
// CList::Clist()
//
// This is the CList class's constructor.
///////////////////////////////////////////////////////////
CList::CList(void)
{
   // Initialize the head and tail pointers.
   list_h = list_t = NULL;
}

///////////////////////////////////////////////////////////
// CList::~CList(void)
//
// This is the CList class's destructor.
///////////////////////////////////////////////////////////
CList::~CList(void)
{
   // Delete all nodes.
   ClearList();
}

///////////////////////////////////////////////////////////
// CList::MakeNewNode()
//
// This function creates a new node and adds it to the
// linked list.
///////////////////////////////////////////////////////////
void CList::MakeNewNode(int n1, int n2)
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
// CList::DisplayList()
//
// This function displays the contents of each node in
// the linked list.
///////////////////////////////////////////////////////////
void CList::DisplayList(void)
{
   node = list_h;
   while (node)
   {
      cout << node->x << ',' << node->y << '\n';
      node = node->next;
   }
}

///////////////////////////////////////////////////////////
// CList::ClearList()
//
// This function deletes all the nodes from the linked
// list.
///////////////////////////////////////////////////////////
void CList::ClearList(void)
{
   while (list_h)
   {
      node = list_h;
      list_h = list_h->next;
      delete node;
   }
}

///////////////////////////////////////////////////////////
// CList::TransferList()
//
// This function copies one linked list to another,
// leaving the source list empty.
///////////////////////////////////////////////////////////
void CList::TransferList(CList &list2)
{
   list2.ClearList();
   list2.list_h = list_h;
   list2.list_t = list_t;
   list_h = NULL;
   list_t = NULL;
}

///////////////////////////////////////////////////////////
// CList::HasNodes()
//
// This function returns TRUE if a list contains any
// nodes and FALSE otherwise.
///////////////////////////////////////////////////////////
int CList::HasNodes(void)
{
   return (list_h != NULL);
}

///////////////////////////////////////////////////////////
// CList::GetNode()
//
// This function returns the contents of a node and then
// deletes the node.
///////////////////////////////////////////////////////////
void CList::GetNode(int &c, int &r)
{
   if (list_h)
   {
      node = list_h;
      c = node->x;
      r = node->y;
      list_h = list_h->next;
      if (!list_h)
         list_t = NULL;
      delete node;
   }
}

