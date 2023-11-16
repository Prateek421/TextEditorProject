#ifndef NODE_H
#define NODE_H
#include <iostream>

class Node
{
public:
    Node(std::string data);
    std::string data;
    Node* next;
};

#endif // NODE_H
