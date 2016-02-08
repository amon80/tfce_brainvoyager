#ifndef QUEUE_H
#define QUEUE_H

//Structures
struct Node {
	int data;
	struct Node* next;
};

typedef struct queue{
    struct Node* front;
    struct Node* rear;
} Queue, *QueuePtr;

//methods
QueuePtr newQueue();
void Enqueue(QueuePtr q, int x);
void Dequeue(QueuePtr q);
int Front(QueuePtr q);
int isEmpty(QueuePtr q);        
//end
#endif //QUEUE_H           
