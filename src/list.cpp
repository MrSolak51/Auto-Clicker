#include "list.h"

Node* List::CreateNode(std::string header, std::string description, float amount, int paid, std::string date){
  Node* node = new Node;
  node->header = header;
  node->description = description;
  node->amount = amount;
  node->paid = paid;
  node->date = date;
  node->next = NULL;
  return node;
}


List::List(){
  size = 0;
}


void List::clear(){
  head = NULL;
  size = 0;
}


void List::AddNode(std::string header, std::string description, float amount, bool paid, std::string date){    
  Node * newNode = this->CreateNode(header, description, amount, paid, date);
  size++;
  if (this->head == NULL) {
    this->head = newNode;
    return;
  }
  Node *temp = this->head;

  while (temp->next != NULL) {
    temp = temp->next;
  }
  newNode->next = temp->next;
  temp->next = newNode;
}


void List::DeleteNode(int index){
  if (this->head == NULL) return;
  Node *temp = this->head;  
  if (index == 0) {
    this->head = temp->next;
    delete temp;
    return;
  }  
  int i = 0;
  while (temp != NULL && i < index - 1) {
    temp = temp->next;
    i++;
  }

  if (temp == NULL || temp->next == NULL) {
      return;
  }
  Node *node_to_delete = temp->next;
  temp->next = temp->next->next;
  delete node_to_delete;
  size--;
}


void List::EditNode(int index, std::string header, std::string description, int amount, bool paid, std::string date){
  Node * temp = this->head;
  int i = 0;
  while (temp != NULL)
  {
      if (index == i)
      {
          temp->header = header;
          temp->description = description;
          temp->amount = amount;
          temp->paid = paid;
          temp->date = date;
      }
      temp = temp->next;
      i++;
  }
}


Node * List::get(int index) {
  Node * temp = this->head;
  int i = 0;
  while (temp != NULL)
  {
      if (index == i)
      {
          return temp;
      }
      temp = temp->next;
      i++;
  }
  
  return NULL;
}


Node * List::get() {
  return this->head;
}


List::~List(){
  delete head;
}

