/* Shawn Hustins
 * ID: 0884015
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "AVLTree.h"

int compNum(void * A, void * B);

void printNum(void * A);


int main(int argc, char * argv[]) {
	
	srand(time(NULL));
	Tree tree; 
	
	newTree(&tree, sizeof(int), compNum, NULL);
	
	unsigned int nums[512];
	
	for (int i = 0; i < 512; ++i)
		nums[i] = rand();
		
	
	printf("\n-------------------------------------------------------------\n"
		   "Inserting set into AVL tree.\n"
		   "-------------------------------------------------------------\n\n");
	
	for (int i = 0; i < 512; ++i)
		insertNode(&tree, nums + i); // no error checking because duplicates will     
		                             // definitely return errors
	printTree(&tree, printNum);
	
	printf("\n-------------------------------------------------------------\n"
		   "Test retrieval from AVL tree.\n"
		   "-------------------------------------------------------------\n\n");
	
	printf("Insert a key to retrieve or NaN to skip test: ");
	
	int num;
	
	while (scanf("%d", &num) == 1) {
		void * rc = retrieveNode(&tree, &num);
		if (rc) {
			printf("%d retrieved.\n", *((int *)rc));
		} else {
			printf("%d not found.\n", num);
		}
		printf("Insert another key to retrieve or NaN to end test: ");
	}
	
	printf("\n-------------------------------------------------------------\n"
		   "Test removals from AVL tree.\n"
		   "-------------------------------------------------------------\n\n");
	
	for (int i = 0; i < 511; ++i) {  // If tree is "empty" printing is supressed
		removeNode(&tree, nums + i); // so one element is left in it
	}
	
	printTree(&tree, printNum);
	
	printf("\n-------------------------------------------------------------\n"
		   "All removals complete. Tests ran successfully.\n"
		   "-------------------------------------------------------------\n\n");
	
	destroyTree(&tree);
	
	return 0;
}


int compNum(void * A, void * B) {
	
	if (!A) return -1;
	if (!B) return 1;
	
	return *((int *)A) - *((int *)B);
}


void printNum(void * A) {
	
	if (A) {
		printf("(%d)", *((int *)A));
	} else {
		printf("(X)");
	}
}