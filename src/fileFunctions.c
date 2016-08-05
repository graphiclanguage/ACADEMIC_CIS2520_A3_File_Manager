/* Shawn Hustins
 * ID: 0884015
 */

#include "fileManager.h"

/* Program Modules: ***********************************************************/

void deleteEntry(char * arg, Tree * tree) {
	
	if (arg ? arg[0] == '\0' : 1) {
		printf("No search term specified\n");
		return;
	}
	++arg;
	
	char new[MAX_LEN] = { '\0' };
	strncpy(new, relPath, MAX_LEN);
	
	if (new[strlen(new) - 1] != '/')
			strncat(new, "/", MAX_LEN);
	
	strncat(new, arg, MAX_LEN);
	
	if (new[strlen(new) - 1] == '/') {
		printf("Cannot delete directories\n");
		return;
	}
	
	Entry entry = {.path = new};
	
	int rc = removeNode(tree, &entry);
	
	if (rc == -1) {
		printf("\'%s\' not found\n", new);
	} else {
		printf("Deleting file: \'%s\'\n", new);
		
		memset(new, 0, MAX_LEN);		
		strncpy(new, absPath, MAX_LEN);
		
		if (new[strlen(new) - 1] != '/')
			strncat(new, "/", MAX_LEN);
		
		strncat(new, arg, MAX_LEN);
		
		int pid = fork();
		
		if (pid == 0)
			execlp("/bin/rm", "rm", "-f", new);
	}
}

void findEntry(char * arg, Tree * tree) {
	
	if (!tree->root) {
		printf("'%s' not found\n", arg);
		return;
	}
	
	if (arg ? arg[0] == '\0' : 1) {
		printf("No search term specified\n");
		return;
	}
	++arg;
	
	char new[MAX_LEN] = { '\0' };
	
	if (arg[0] != '*') {
		strncpy(new, relPath, MAX_LEN);
	
		if (new[strlen(new) - 1] != '/')
			strncat(new, "/", MAX_LEN);
	}
	
	strncat(new, arg, MAX_LEN);
	
	Entry entry = {.path = new};
	int matches = 0;
	
	printf("---------------------------------------\n");
	printf("Searching for \'%s\' in \'%s\'\n", arg, relPath);
	printf("---------------------------------------\n");
	
	if (arg[0] == '*' && arg[strlen(arg) - 1] == '*' && (strlen(arg) > 1)) {
		
		++entry.path;
		entry.path[strlen(entry.path) - 1] = '\0';
		
		findAll(&entry, tree->root, isSubstring, &matches);
		printf("Matches: %d\n", matches);	   

	} else if (arg[strlen(arg) - 1] == '*') {
	
		entry.path[strlen(entry.path) - 1] = '\0';
		findAll(&entry, tree->root, isSubstring, &matches);
		printf("Matches: %d\n", matches);	   

	} else if (arg[0] == '*') {
		
		++entry.path;
		
		findAll(&entry, tree->root, isEndSubstring, &matches);
		printf("Matches: %d\n", matches);
		
	} else {
		
		Entry * pull = retrieveNode(tree, &entry);
		printf("Match: %s\n", pull ? pull->path : "Not found");
		
	}
	printf("---------------------------------------\n");
}

void moveDir(char * arg, Tree * tree) {
	
	if (arg ? arg[0] == '\0' : 1) {
		printf("No directory specified\n");
		return;
	}
	
	char newDir[MAX_LEN];
	
	if (arg[1] == '.' && arg[2] == '.') {
		
		int i;
		int fromEnd = strlen(relPath) - 2;
		
		for (i = fromEnd; i > 0; --i) {
			if (relPath[i] == '/') {
				relPath[i+1] = '\0';
				break;
			}
		}
		if (i == 0) {
			printf("Currently in highest accessible folder.\n");
			return;
		}	
		
		absPath[strlen(absPath) - (fromEnd - i) - 1] = '\0';
			
	} else {
		
		if (arg[strlen(arg) - 1] != '/') {
			printf("Uknown directory, should be appended with '/'\n");
			return;
		}
		strncpy(newDir, relPath, MAX_LEN);

		if (newDir[strlen(newDir) - 1] != '/')
			strncat(newDir, "/", MAX_LEN);
	
		strncat(newDir, ++arg, MAX_LEN);
		
		Entry entry = {.path = newDir};
		Entry * location = retrieveNode(tree, &entry);
	
		if (!location) {
			printf("Unknown directory: \"%s\"\n", newDir);
		} else {
			strncat(absPath, "/", MAX_LEN);
			arg[strlen(arg) - 1] = '\0';
			strncat(absPath, arg, MAX_LEN);
			strncpy(relPath, newDir, MAX_LEN);
		}
	}
}

void renameEntry(char * arg, Tree * tree) {
	
	if (arg ? arg[0] == '\0' : 1) {
		printf("No search term specified\n");
		return;
	}
	++arg;
	
	int i;
	char * split = NULL;
	char oldName[MAX_LEN + strlen(relPath)];
	char newName[MAX_LEN];
	
	strncpy(oldName, relPath, MAX_LEN);
	
	if (oldName[strlen(oldName) - 1] != '/')
			strncat(oldName, "/", MAX_LEN);
	
	strncat(oldName, arg, MAX_LEN);
	
	for (i = 0; i < strlen(oldName) - 1; ++i) {
		if (oldName[i] == ' ') {
			oldName[i] = '\0';
			split = oldName + i + 1;
			break;
		}
	}
	
	if (!split ? 1 : strlen(split) == 0) {
		printf("New name missing\n");
		return;
	}
	
	if (split[strlen(split) - 1] == '/') {
		printf("Cannot move directories\n");
		return;
	}
	
	Entry oldEntry = {.path = oldName};
	
	if (retrieveNode(tree, &oldEntry) == NULL) {
		printf("\'%s\' not found\n", oldName);
		return;
	}
	
	strncpy(newName, relPath, MAX_LEN);
	
	if (newName[strlen(newName) - 1] != '/')
			strncat(newName, "/", MAX_LEN);
	
	strncat(newName, split, MAX_LEN);
	
	int rc;
	int pid = fork();
		
	if (pid == 0)
		execlp("/bin/mv", "mv", oldName, newName);
	
	wait(&rc);
	
	if (WEXITSTATUS(rc) == EXIT_SUCCESS) {
		
		char * newNameDyn = calloc(strlen(newName), sizeof(char));
		if (!newNameDyn) {
			printf("Failure renaming file\n");
			return;
		}
		strcpy(newNameDyn, newName);
		
		Entry newEntry = {.path = newNameDyn};
		
		rc = insertNode(tree, &newEntry);
		if (rc == -1) {
			printf("Failure renaming file\n");
			free(newNameDyn);
			return;
		}
		
		removeNode(tree, &oldEntry);
		printf("\'%s\' -> \'%s\'\n", oldName, newName);
		
	} else {
		printf("Failure renaming file\n");
	}
}

void helpMsg() {
	
	printf("-------------------------------------------------------\n"
	       "CLI File Manager Options: \n\n"
		   "'d'/'delete' : delete a file (not directory)\n"
		   "               $ delete {file}\n\n"
		   "'f'/'find'   : list matches to given expression\n"
		   "               $ find {expression}\n"
		   "               (Pre and postfixed wildcard allowed '*')\n\n"
		   "'h'/'help'   : display program options\n"
		   "               $ help\n\n"
		   "'m'/'move'   : move to another directory\n"
		   "               $ move {directory}\n"
		   "               $ move .. to move up one directory\n\n"
		   "'q'/'quit'   : quit program\n"
		   "               $ q\n\n"
		   "'r'/'rename' : rename or move a file\n"
		   "               $ rename {file} {new name}\n"
		   "-------------------------------------------------------\n");	
}

/* Miscellaneous Program Functions: *******************************************/

int globerr(const char * path, int eerrno) {
	
	fprintf(stderr, "%s: %s\n", path, strerror(eerrno));
	return 0;
}

void globDeeper(char * search, Tree * tree) {
	
	glob_t results;
	
	int rc = glob(search, GLOB_MARK, globerr, &results);
	if (rc != 0) return;
	
	for (int i = 0; i < results.gl_pathc; i++) {
		
		char * this = calloc(strlen(results.gl_pathv[i]) + 1, sizeof(char));
		strcpy(this, results.gl_pathv[i]);
		Entry new = {.path = this};
		insertNode(tree, &new);
	}
	
	for (int i = 0; i < results.gl_pathc; i++) {
		
		char * this = results.gl_pathv[i];
			
		if (this[strlen(this) - 1] == '/') {
			char * temp = calloc(strlen(this) + 2, sizeof(char));
			if (!temp) break;
			strcpy(temp, this);
			temp[strlen(temp)] = '*';
			
			globDeeper(temp, tree);
			
			free(temp);
		}
	}
	
	globfree(&results);
}

int compFile(void * A, void * B) {
	
	char * one = ((Entry *)A)->path;
	char * two = ((Entry *)B)->path;
	int length = strlen(one) > strlen(two) ? strlen(one) : strlen(two);
	
	for (int i = 0; i < length; ++i) {
		if (one[i] == '*') break;
		int cmp = one[i] - two[i];
		if (cmp) return cmp;
	}
	return 0;
}

int isSubstring(void * A, void * B) {
	
	char * one = ((Entry *)A)->path;
	char * two = ((Entry *)B)->path;
	
	if (strstr(two, relPath) != NULL)
		if (strstr(two, one) != NULL) 
			return 0;
	
	return 1;
}

int isEndSubstring(void * A, void * B) {
	
	char * one = ((Entry *)A)->path;
	char * two = ((Entry *)B)->path;
	char newA[MAX_LEN];
	char newB[MAX_LEN];
	
	strncpy(newA, one, MAX_LEN);
	strncpy(newB, two, MAX_LEN);
	strncat(newA, "//", MAX_LEN);
	strncat(newB, "//", MAX_LEN);
	
	if (strstr(newB, relPath) != NULL)
		if (strstr(newB, newA) != NULL) 
			return 0;
	
	return 1;
}

void freeFile(void * A) {
	
	free(((Entry *)A)->path);
}	

void findAll(void * arg, Node * node, Comparator comparator, int * matches) {
	
	if(!node) return;

	findAll(arg, node->left, comparator, matches);

	if (!node->deleted) {
		if (comparator(arg, node->data) == 0) {
			Entry * this = node->data;
			printf("-- %s\n", this->path);
			++*matches;
		}
	}
	
	findAll(arg, node->right, comparator, matches);
}