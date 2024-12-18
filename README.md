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