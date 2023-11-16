#include "linkedlist.h"
#include <iostream>



LinkedList::LinkedList()
{
    head=NULL;
    length=0;

}

void LinkedList::insert(std::string data)
{
    Node* newnode=new Node(data);
    if (head==NULL)
    {
            head=newnode;
            length=1;
            return;
    }
    Node* temp=head;
    while(temp->next!=NULL)
    {
        temp=temp->next;
    }
    temp->next=newnode;
    length++;
    return;
}

int LinkedList::swap(int m,int n)
{
    if(m>length || n>length)
    {
            return 0;
    }
    Node* temp1=this->head;
    Node* temp2=this->head;
    while(m-- >1)
    {
        temp1=temp1->next;
    }
    while(n-- >1)
    {
        temp2=temp2->next;
    }
    std::string temp=temp1->data;
    temp1->data=temp2->data;
    temp2->data=temp;
    return 1;
}

std::string LinkedList:: getData()
{
    std::string text="";
    Node* temp=head;
    while(temp!=NULL)
    {
            text=text+temp->data+". ";
    }
    return text;
}
