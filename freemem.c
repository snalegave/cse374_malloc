//freemem.c
#include <inttypes.h>
#include "mem.h"
#include "mem_impl.h"
#include "externs.h"

void freemem(void * p);
void combineSmallBlocks(memNode * p, memNode * prev);
memNode * findMemorySpot(memNode * p);
void addToFree(memNode * p, memNode * prev);

//will free the block of memory the passed in pointer points to
void freemem(void * p) {
	if (!p) {
		return;
	} 
	memNode* pNode = p - sizeof(memNode);
	memNode * prev = findMemorySpot(pNode);
	addToFree(pNode, prev);
	freeMem = freeMem + pNode->size + sizeof(memNode);
	totalFree = totalFree + 1;
	combineSmallBlocks(pNode, prev);
}

void combineSmallBlocks(memNode * p, memNode * prev) {
	if (p->next) {
		memNode * pNext = (memNode *) p->next;
		if ((long) p + p->size + sizeof(memNode) - (long) pNext == 0) {
			if ((int) p->size + (int) pNext->size >= 128) {
				p->size = p->size + pNext->size + 2*sizeof(memNode);
				if (pNext->next) {
					p->next = pNext->next;
				} else {
					p->next = (uintptr_t) NULL;
				}
				totalFree = totalFree - 1;
			}
		}
	}
	if (prev) {
		if ((long) prev + prev->size+16 - (long) p == 0) {
			if ((int) prev->size + (int) p->size >= 128) {
				prev->size = prev->size + p->size + 32;
				if (p->next) {
					prev->next = p->next;
				} else {
					prev->next = (uintptr_t) NULL;
				}
				totalFree = totalFree - 1;
			}
		}
	}
}

//returns a pointer to the data block that appears before the passed in data block in memory
//returns null if p should be added front of free
memNode * findMemorySpot(memNode * p) {
	if (!root || root > p) {
		return NULL;
	}
	memNode * node = root;
	//address of next size is &root + (root->size+2)/8
	//difference between 2 addresses in bytes is a1 + root->size+16 - a2 
	while (node->next) {
		if ((memNode *) node->next < p) {
			node = (memNode *) node->next;
		} else {
			return node;
		}
	}
	return node;
}

void addToFree(memNode * p, memNode * prev) {
	if (!prev) {
		if (root) {
			p->next = (uintptr_t) root;
			root = p;
		} else {
			root = p;
		}
	} else if (!prev->next) {
		prev->next = (uintptr_t) p;
	} else {
		p->next = prev->next;
		prev->next = (uintptr_t) p;
	}
}
