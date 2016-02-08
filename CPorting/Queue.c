#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

QueuePtr newQueue(){
    QueuePtr toReturn = (QueuePtr) malloc(sizeof(Queue));
    toReturn->front = NULL;
    toReturn->rear = NULL;
    return toReturn;
}


void Enqueue(QueuePtr q, int x) {
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	temp->data =x; 
	temp->next = NULL;
	if(q->front == NULL && q->rear == NULL){
		q->front = q->rear = temp;
		return;
	}
	q->rear->next = temp;
	q->rear = temp;
}

//We are programmers, we are not so stupid to search for front when queue is empty
void Dequeue(QueuePtr q) {
	struct Node* temp = q->front;
	if(q->front == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(q->front == q->rear) {
		q->front = q->rear = NULL;
	}
	else {
		q->front = q->front->next;
	}
	free(temp);
}

//We are programmers, we are not so stupid to search for front when queue is empty
int Front(QueuePtr q) {
	return q->front->data;
}

int isEmpty(QueuePtr q){
	return q->front == q->rear;
}
