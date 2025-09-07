#ifndef LIST_H
#define LIST_H

#include "node.h"

class List
{
private:
  Node * head = NULL;
  Node* CreateNode(std::string header, std::string description, float amount, int paid, std::string date);
public:
  int size = 0;
  List();
  void clear();
  void AddNode(std::string header, std::string description, float amount, bool paid, std::string date);
  void DeleteNode(int index);
  void EditNode(int index, std::string header, std::string description, int amount, bool paid, std::string date);
  Node * get(int index);
  Node * get();
  ~List();
};


#endif // LIST_H
