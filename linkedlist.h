#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "node.h"

class LinkedList
{
public:
    Node* head;
    int length;
    LinkedList();


    void insert(std::string data);
    std::string getData();
    int swap(int m, int n);

};

#endif // LINKEDLIST_H
