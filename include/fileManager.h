/* Shawn Hustins
 * ID: 0884015
 */

#ifndef _fileManager_h_
#define _fileManager_h_

#include "AVLTree.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN 128 

// We were asked specifically to make a struct to store all the data we need to 
// know about the entries. Well, this is all I need:
typedef struct {
	char * path;
} Entry;
// So as stupid as this looks, I'm following instructions correctly

char * relPath;
char * absPath;

/* Program Modules: ***********************************************************/

/*
 * deleteEntry
 * Takes a user specified file and attempts to delete it. 
 */
void deleteEntry(char * arg, Tree * tree);

/*
 * findEntry
 * Takes user specified search term and returns matches. If no wildcards are
 * used, the exact the term will be searched for efficiently. If wildcards are
 * specified all possible matches will be searched for.
 */
void findEntry(char * arg, Tree * tree);

/*
 * moveDir
 * Attempts to move to the user specified folder relative to current location.
 * If '..' is specified, will attempt to move up a directory.
 */
void moveDir(char * arg, Tree * tree);

/*
 * renameEntry
 * Takes user specified file and renames it, can be moved between directories.
 */
void renameEntry(char * arg, Tree * tree);

/*
 * helpMsg
 * Prints out all options for program.
 */
void helpMsg();

/* Internal Program Functions: ************************************************/

/*
 * globErr
 * Function to handle glob errors.
 */
int globerr(const char *path, int eerrno);

/*
 * globDeeper
 * Takes a file path to be searched and pulls all files within all subfolders
 */
void globDeeper(char * search, Tree * tree);

/*
 * compFile
 * Comparison function for AVL tree library, will return 0 (equal) if A and B
 * are identical or if A contains a wildcard '*'. Will return > 0 if A > B or
 * < 0 if A < B
 */
int compFile(void * A, void * B);

/*
 * isSubstring
 * Alternative comparison function for AVL tree library, will return 0 (equal)
 * if A is a substring of B.
 */
int isSubstring(void * A, void * B);

/*
 * isEndSubstring
 * Alternative comparison function for AVL tree library, will return 0 (equal)
 * if A is a substring found at the end of B.
 */
int isEndSubstring(void * A, void * B);

/*
 * freeFile
 * Destructor function for AVL tree library, frees referenced dynamic memory in
 * Entry struct
 */
void freeFile(void * A);

/*
 * findAll
 * Full tree traversal function. Used for wildcard searches.
 */
void findAll(void * arg, Node * node, Comparator comparator, int * matches);

#endif