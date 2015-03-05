//
//  bench.c
//  hw6
//
//  Created by Max Golub on 2/25/15.
//  Copyright (c) 2015 Max Golub. All rights reserved.
//

#include <stdio.h>
#include "mem.h"
#include "mem_impl.h"
#include <unistd.h> //command line parsing
//small and large block limits
#define def_ntrial 10000
#define def_pctget 50
#define def_pctlarge 10
#define def_small_limit 200
#define def_large_limit 20000

extern memNode * root;

//function prototypes
int getRandom(int size); //reads from urandom for size
void runRandomOp(); //run ethier a getmem or freemem op
void freeRandom(int allocs[], int * size); //free a random block


int main(int argc, const char * argv[]) {
    // run the main memory testing code
    //create a array of ntrial size to keep track of allocs
    //as well as inc numbe rof allocs
    //delete
    //
	//root=malloc(1024);
	//root->size = 16;
	//printf("%zu\n", sizeof(uintptr_t *));
	//printf("%zu\n", sizeof(*root));
	//printf("%zu\n", root->size);
	//printf("%p  %lu\n", &root, root + (root->size+16) - root);
    printf("Hello, memory!\n");
    //temp getmem tests weee
    int * a;
    //print_heap(stdout);
    a = (int *) getmem(32);
    //print_heap(stdout);
    a[31] = 1;
    a[32] = 1;
    printf("%lu, %d\n", sizeof(*a), a[31]);
    int * b;
    b = (int *) getmem(14);
    //print_heap(stdout);
    //freemem();
    char * c = getmem(463);
    //print_heap(stdout);
    char * d = getmem(45);
    printf("a: %lu, b: %lu, c: %lu, d: %lu\n", sizeof(*a), sizeof(*b), sizeof(*c), sizeof(*d));
    print_heap(stdout);
	printf("new\n");
	freemem(c);
	print_heap(stdout);
	freemem(d);
	print_heap(stdout);
	freemem(a);
	print_heap(stdout);
	freemem(b);
	print_heap(stdout);
	//print_heap(stdout);
    int * lol = getmem(3241234124);
	freemem(lol);
    print_heap(stdout);
    return 0;
}

int getRandom(int size) {
    return 0;
}

void runRandomOp() {
    //do something random
}


void freeRandom(int allocs[], int * size) {
    //do something like freeing. But random!
}