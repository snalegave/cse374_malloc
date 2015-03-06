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
//used for get stats
int totalSize = 0;
int totalFree = 0;
int nFreeBlocks = 0;

//grab at least size amount of mem and return a void pointer to the user
void* getmem(uintptr_t size) {
    if (!root) {
        root = mallocData(root, LARGE_BLOCK_SIZE);
        nFreeBlocks += 1;
    }
    memNode * choosenBlock = chooseBlock(root, NULL, size);
    nFreeBlocks -= 1;
    totalFree -= choosenBlock-> size;
	printf("CHOSEN: 0x%08lx\n", choosenBlock);
    return (void *) choosenBlock+sizeof(memNode); //should be start of data
}

//return
memNode * splitBlock(memNode * block, uintptr_t splitSize) {
    splitSize = align16(splitSize);
    uintptr_t oldSize = block->size;
    uintptr_t oldBlockNext = block->next;
    block->size = splitSize;
    block->next = (uintptr_t) block+splitSize+sizeof(memNode);
	//???
	
	
    memNode newBlock;
    newBlock.size = oldSize-splitSize-sizeof(memNode); // set the new size.
    newBlock.next = oldBlockNext;
    *(memNode *)(block->next) = newBlock; //lol wat (does this werk?)
	//???
    nFreeBlocks += 1;
    totalFree -= sizeof(memNode);
    return block;
}

memNode * chooseBlock(memNode * block, memNode * prevBlock, uintptr_t size) {
    if (block->size < size) {
        if (block->next) {
			printf("RECURSE\n");
           return chooseBlock((memNode *) block->next, block, size);
        }
        else {
            memNode * newBlock = mallocData((memNode *)block->next, size);
            block->next = (uintptr_t) newBlock; //link the new block
            return chooseBlock(newBlock, block ,size);
        }
    }
    else if (block->size > size + maxOveragePercent*size && block->size > size + (16 - size % 16) + 16) {
        block = splitBlock(block, size);
		printf("SPLIT\n");
		print_heap(stdout);
    }
    if (prevBlock) {
        prevBlock->next = block->next; //remove block from linked list
    }
    else {
        root = (memNode *) block->next; //block->next points to not free memory
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
    }
    return num;
}

void removeFromFree(memNode * block) {
    //remove a memNode wheee
}

memNode * mallocData(memNode * block, uintptr_t size) {
    //although malloc does this, the exact size will not be known
    size = align16(size);
    block = malloc(size+sizeof(memNode));
    block->size = size;
    totalSize += size;
    totalFree += size;
    nFreeBlocks += 1;
    return block;
}