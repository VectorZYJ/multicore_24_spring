#ifndef PROJECTCODE_MYDEQUE_H
#define PROJECTCODE_MYDEQUE_H

#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int value;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node* head;
    Node* tail;
} MyDeque;

void initDeque(MyDeque* deque) {
    deque->head = deque->tail = NULL;
    deque->size = 0;
}

void destroyDeque(MyDeque* deque) {
    Node* current = deque->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    deque->size = 0;
}

void push_front(MyDeque* deque, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->prev = NULL;

    newNode->next = deque->head;
    deque->size++;
    if (deque->head != NULL) deque->head->prev = newNode;
    else deque->tail = newNode;
    deque->head = newNode;
}

void push_back(MyDeque* deque, int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;

    newNode->prev = deque->tail;
    deque->size++;
    if (deque->tail != NULL) deque->tail->next = newNode;
    else deque->head = newNode;
    deque->tail = newNode;
}

int pop_front(MyDeque* deque) {
    if (deque->head == NULL) return -1;
    Node* temp = deque->head;
    int value = temp->value;
    deque->head = deque->head->next;
    if (deque->head != NULL) deque->head->prev = NULL;
    else deque->tail = NULL;
    free(temp);
    deque->size--;
    return value;
}

int pop_back(MyDeque* deque) {
    if (deque->tail == NULL) return -1;
    Node* temp = deque->tail;
    int value = temp->value;
    deque->tail = deque->tail->prev;
    if (deque->tail != NULL) deque->tail->next = NULL;
    else deque->head = NULL;
    free(temp);
    deque->size--;
    return value;
}


#endif //PROJECTCODE_MYDEQUE_H
