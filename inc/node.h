#ifndef NODE_H
#define NODE_H

#include <iostream>

typedef struct Node
{
    std::string header;
    std::string description;
    float amount;
    bool paid;
    std::string date;
    Node * next;
} Node;

#endif // NODE_H
