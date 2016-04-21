
#ifndef INT_LIST_H
#define INT_LIST_H


typedef struct Node *Node;
typedef struct List *List;

extern Node newList(int index);
extern void addhi(Node *tail, int index);
extern int remlo(Node *head, int index);
extern int isEmpty();


#endif