#ifndef PROJECTCODE_CONCURRENTDEQUE_H
#define PROJECTCODE_CONCURRENTDEQUE_H

#include <omp.h>
//#include "/opt/homebrew/opt/libomp/include/omp.h"

typedef struct Node_c {
    int value;
    struct Node_c* prev;
    struct Node_c* next;
} Node_c;

typedef struct {
    int size;
    Node_c* head;
    Node_c* tail;
    omp_lock_t headLock;
    omp_lock_t tailLock;
} ConcurrentDeque;

void initDeque_c(ConcurrentDeque* deque) {
    deque->head = deque->tail = NULL;
    deque->size = 0;
    omp_init_lock(&deque->headLock);
    omp_init_lock(&deque->tailLock);
}

void destroyDeque_c(ConcurrentDeque* deque) {
    Node_c* current = deque->head;
    while (current != NULL) {
        Node_c* temp = current;
        current = current->next;
        free(temp);
    }
    deque->size = 0;
    omp_destroy_lock(&deque->headLock);
    omp_destroy_lock(&deque->tailLock);
}

void push_front_c(ConcurrentDeque* deque, int value) {
    Node_c* newNode = (Node_c*)malloc(sizeof(Node_c));
    newNode->value = value;
    newNode->prev = NULL;

    omp_set_lock(&deque->headLock);
    newNode->next = deque->head;
    deque->size++;
    if (deque->head != NULL) deque->head->prev = newNode;
    else deque->tail = newNode;
    deque->head = newNode;
    omp_unset_lock(&deque->headLock);
}

void push_back_c(ConcurrentDeque* deque, int value) {
    Node_c* newNode = (Node_c*)malloc(sizeof(Node_c));
    newNode->value = value;
    newNode->next = NULL;

    omp_set_lock(&deque->tailLock);
    newNode->prev = deque->tail;
    deque->size++;
    if (deque->tail != NULL) deque->tail->next = newNode;
    else deque->head = newNode;
    deque->tail = newNode;
    omp_unset_lock(&deque->tailLock);
}

int pop_front_c(ConcurrentDeque* deque) {
    omp_set_lock(&deque->headLock);
    if (deque->head == NULL) {
        omp_unset_lock(&deque->headLock);
        return -1;
    }
    Node_c* temp = deque->head;
    int value = temp->value;
    deque->head = deque->head->next;
    if (deque->head != NULL) deque->head->prev = NULL;
    else deque->tail = NULL;
    free(temp);
    deque->size--;
    omp_unset_lock(&deque->headLock);
    return value;
}

int pop_back_c(ConcurrentDeque* deque) {
    omp_set_lock(&deque->tailLock);
    if (deque->tail == NULL) {
        omp_unset_lock(&deque->tailLock);
        return -1;
    }
    Node_c* temp = deque->tail;
    int value = temp->value;
    deque->tail = deque->tail->prev;
    if (deque->tail != NULL) deque->tail->next = NULL;
    else deque->head = NULL;
    free(temp);
    deque->size--;
    omp_unset_lock(&deque->tailLock);
    return value;
}

#endif //PROJECTCODE_CONCURRENTDEQUE_H
