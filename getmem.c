 //
//  getmem.c
//  hw6
//
//  Created by Max Golub on 2/25/15.
//  Copyright (c) 2015 Max Golub. All rights reserved.
//
#include <inttypes.h>
#include <stdlib.h>
#include "mem_impl.h"
#include "mem.h"

#define ALIGN_VAL 16
#define SPLIT_SIZE 256 //what size we split a block at
#define LARGE_BLOCK_SIZE 1024 * 16 //size of initial block
#define maxOveragePercent .20

//funtion protos
memNode * splitBlock(memNode * block, uintptr_t Splitsize); //split a block into many blocks
void removeFromFree(memNode * block); //remove a memnode from the freelist
memNode * mallocData(memNode * block, uintptr_t size); //give memory to a block
memNode * chooseBlock(memNode * block, memNode * prevBlock, uintptr_t size);
uintptr_t align16(uintptr_t num);
//globals
memNode * root;

int totalFree = 0;
int usedMem = 0;
int freeMem = 0;

//grab at least size amount of mem and return a void pointer to the user
void* getmem(uintptr_t size) {
    if (!root) {
        root = mallocData(root, 0);
        //print_heap(stdout);
        //current = root;
    }
    memNode * choosenBlock = chooseBlock(root, NULL, size);
    return (void *) choosenBlock+sizeof(memNode); //should be start of data
}

//return
memNode * splitBlock(memNode * block, uintptr_t splitSize) {
    splitSize = align16(splitSize);
    uintptr_t oldSize = block->size;
    block->size = splitSize;
    block->next = (uintptr_t) block+splitSize+sizeof(memNode); //maybe want a define for fields?
    memNode newBlock;
    newBlock.size = oldSize-splitSize-sizeof(memNode); // set the new size.
    newBlock.next = (uintptr_t) NULL;
    *(memNode *)(block->next) = newBlock; //lol wat (does this werk?)
    //print_heap(stdout);
    return block; //do we need to return this, or should we figure out a new way
}

memNode * chooseBlock(memNode * block, memNode * prevBlock, uintptr_t size) {
    if (block->size < size) {
        if (block->size && block->next) {
           return chooseBlock((memNode *) block->next, block, size);
        }
        else {
            memNode * newBlock = mallocData((memNode *)block->next, size);
            block->next = (uintptr_t) newBlock; //link the new block
            return chooseBlock(newBlock, NULL ,size);
        }
    }
    else if (root->size > maxOveragePercent*size) {
        splitBlock(block, size);
    }
    if (prevBlock) {
        prevBlock->next = block->next; //remove block from linked list
    }
    else {
        root = (memNode *) block->next;
    }
	block->next = (uintptr_t) NULL;
    return block;
}

//align a number to 16
uintptr_t align16(uintptr_t num) {
    int alignment = num % ALIGN_VAL;
    if(num < ALIGN_VAL) {
        num = ALIGN_VAL;
    }
    else if (alignment) {
        num = num + ALIGN_VAL - alignment;
        printf("%lu\n", num % ALIGN_VAL);
    }
    return num;
}

void removeFromFree(memNode * block) {
    //remove a memNode wheee
}

memNode * mallocData(memNode * block, uintptr_t size) {
    if (size > LARGE_BLOCK_SIZE) {
        //although malloc does this, the exact size will not be known
        size = align16(size);
        block = malloc(size+sizeof(memNode));
        block->size = size;
    }
    else {
        block = malloc(LARGE_BLOCK_SIZE);
        block->size = LARGE_BLOCK_SIZE-sizeof(memNode); //should be 16 less
    }
    printf("%d \n ", LARGE_BLOCK_SIZE);
    return block;
}