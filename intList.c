

#include "intList.h"

struct Node {
	int index;
	Node *next;
};


struct List {
	Node *head;
	Node *tail;
};



Node newList(int index)
{
	List reusable_indices = malloc(List);
	reuseable_indices->head = NULL;
	reuseable_indices->tail = NULL;
}
extern void addhi(Node *tail, int index);
extern int remlo(Node *head, int index);
extern int isEmpty();



