///////////////////////////////////////////////////////////
// LIST1.CPP: Linked list demonstration program 1.
///////////////////////////////////////////////////////////

#include <iostream.h>
#include <conio.h>

// Node structure.
struct Node
{
   int x, y;
   Node *next;
};

// Node pointers.
Node *node = NULL,
     *list_h = NULL,
     *list_t = NULL;

void main(void)
{
   for (int i = 0; i < 10; ++i)
   {
      node = new Node;
      node->x = i;
      node->y = i * 10;
      if (!list_h)
         list_h = node;
      else
         list_t->next = node;
      list_t = node;
      list_t->next = NULL;
   }
   while (list_h)
   {
      node = list_h;
      list_h = list_h->next;
      cout << node->x << ',' << node->y << '\n';
      delete node;
   }
   getch();
}
