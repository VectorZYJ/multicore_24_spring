/* File:     test.c
 * Purpose:  Test normal deque and concurrent deque, using trivial instructions or brute BFS
 *
 * Compile:  module avail (check available module)
 *           module load gcc-12.2 (the newest version)
 *           gcc -Wall -std=c99 -fopenmp -o test test.c
 * Run:      ./test <n> <test> <opt> <t>
 *           'n' is problem size
 *           'test' is type of testing case, 1 stands for trivial instruction test, 2 stands for graph BFS test
 *           'opt' determines whether you use multiple threads, 0 stands for single thread, 1 stands for multiple threads
 *           't' is the number of threads
 * Testing:  time ./test <n> <test> <opt> <t>
 *
 * Input:    none
 * Output:   Running result
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "MyDeque.h"
#include "ConcurrentDeque.h"

void test_bfs(int n, int opt, int t) {
    int* visit;
    int num_traversed = 0;
    visit = (int*)malloc(n*sizeof(int));
    for (int i=0; i<n; i++)
        visit[i] = 0;
    if (opt == 0) {
        MyDeque deque;
        initDeque(&deque);
        push_back(&deque, 0);
        visit[0] = 1;
        while (deque.size > 0) {
            int u = deque.head->value;
            num_traversed++;
            for (int v=0; v<n; v++)
                if (u != v && visit[v] == 0) {
                    push_back(&deque, v);
                    visit[v] = 1;
                }
            pop_front(&deque);
        }
        destroyDeque(&deque);
        printf("BFS on complete graph with %d nodes finished.\n", n);
    }

    else if (opt == 1) {
        ConcurrentDeque deque;
        initDeque_c(&deque);
        push_back_c(&deque, 0);
        visit[0] = 1;
        while (deque.size > 0) {
            int u = deque.head->value;
            num_traversed++;
#pragma omp parallel for num_threads(t)
            for (int v=0; v<n; v++)
                if (u != v && visit[v] == 0) {
                    push_back_c(&deque, v);
                    visit[v] = 1;
                }
            pop_front_c(&deque);
        }
        destroyDeque_c(&deque);
        printf("BFS on complete graph with %d nodes finished. Using %d threads.\n", n, t);
    }
}


void test_triv(int n, int opt, int t) {
    if (opt == 0) {
        MyDeque deque;
        initDeque(&deque);
        for (int i=0; i<n; i++) {
            push_back(&deque, i);
            push_front(&deque, i);
            pop_front(&deque);
            pop_back(&deque);
        }
        destroyDeque(&deque);
        printf("Trivial test for deque with %d instructions completed.\n", 4*n);
    }

    if (opt == 1) {
        ConcurrentDeque deque;
        initDeque_c(&deque);
#pragma omp parallel for num_threads(t)
        for (int i=0; i<n; i++) {
            push_back_c(&deque, i);
            push_front_c(&deque, i);
            pop_front_c(&deque);
            pop_back_c(&deque);
        }
        destroyDeque_c(&deque);
        printf("Trivial test for deque with %d instructions completed. Using %d threads.\n", 4*n, t);
    }
}


int main(int argc, char* argv[]) {
    int n = strtol(argv[1], NULL, 10);
    int test = strtol(argv[2], NULL, 10);
    int opt = strtol(argv[3], NULL, 10);
    int t = strtol(argv[4], NULL, 10);
    if (test == 1) test_triv(n, opt, t);
    if (test == 2) test_bfs(n, opt, t);
    return 0;
}
