#include <stdio.h>
#include <stdlib.h>
#include <glob.h> 
#include <string.h>
#include "AVLTree.h"

// We were asked to make a struct to store all the data we need to know about
// the entries. Well, this is all I need:
typedef struct {
	char * path;
	unsigned int modified : 1;
} Entry;

int globerr(const char *path, int eerrno);

void globDeeper(char * search, Tree * tree);

int compFile(void * A, void * B);

int alphabetize(char * A, char * B);

void freeFile(void * A);

void printFile(void * A);

int main(int argc, char * argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s (directory, \".\" for current)\n", argv[0]);
		exit(1);
	}
	
	putchar('\n');
	
	Tree fileTree;
	
	newTree(&fileTree, sizeof(Entry), compFile, freeFile);
	
	char * searchPath = calloc(strlen(argv[1]) + 1, sizeof(char));
	strcpy(searchPath, argv[1]);
	searchPath[strlen(argv[1])] = '*';
	searchPath[strlen(argv[1]) + 1] = '\0';
	
	globDeeper(searchPath, &fileTree);
	
	printTree(&fileTree, printFile);
	
	destroyTree(&fileTree);
	free(searchPath);

	putchar('\n');
	
	return 0;
}

int globerr(const char * path, int eerrno) {
	
	fprintf(stderr, "%s: %s\n", path, strerror(eerrno));
	return 0;
}

void globDeeper(char * search, Tree * tree) {
	
	glob_t results;
	
	int rc = glob(search, GLOB_MARK | GLOB_ERR, globerr, &results);
	
	if (rc != 0) return;
	
	for (int i = 0; i < results.gl_pathc; ++i) {
		
		char * this = calloc(strlen(results.gl_pathv[i]), sizeof(char));
		strcpy(this, results.gl_pathv[i]);
		printf("New: %s\n", this);
		Entry new = {.path = this};
		insertNode(tree, &new);
		
		if (this[strlen(this) - 1] == '/') {
			
			char * temp = calloc(strlen(this) + 1, sizeof(char));
			strcpy(temp, this);
			temp[strlen(temp)] = '*';
			temp[strlen(temp) + 1] = '\0';
			
			globDeeper(temp, tree);
			
			free(temp);
		}
	}
	
	globfree(&results);
}

int compFile(void * A, void * B) {
	
	return alphabetize(((Entry *)A)->path, ((Entry *)B)->path);
}

int alphabetize(char * A, char * B) {
	
	if (!A && !B) return 0;
	
	if (A[0] == B[0]) return alphabetize(++A, ++B);
	
	return A[0] - B[0];
}

void freeFile(void * A) {
	
	free(((Entry *)A)->path);
}

void printFile(void * A) {
	
	printf("%s", ((Entry *)A)->path);
}