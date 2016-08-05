/* Shawn Hustins
 * ID: 0884015
 */

#include "fileManager.h"

int main(int argc, char * argv[]) {
	
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s (directory, \".\" for current)\n", argv[0]);
		exit(1);
	}	
	// Initialization:
	char path[128];
	char * name = getlogin();
	relPath = argv[1];
	absPath = realpath(relPath, path);
	
	
	Tree fileTree;
	newTree(&fileTree, sizeof(Entry), compFile, freeFile);
	globDeeper(relPath, &fileTree);
	
	// Main Program Loop (Uses same user input framework from my A2 submission)
	printf("------------------------------------\n"
		   "CLI File Manager: Enter 'h' for help\n"
		   "------------------------------------\n");
	printf("%s:%s$ ", name, absPath);
	
	char buffer[128];
	char * arg = NULL;
	
	while (strcmp(fgets(buffer, 128, stdin), "q\n")) {
		buffer[strlen(buffer) - 1] = '\0'; // Eliminate newline from argument
		arg = strchr(buffer, ' '); // Pull additional arguments
		
		switch (buffer[0]) {	
			case 'd': // Delete: delete single entry
				deleteEntry(arg, &fileTree);
				break;
				
			case 'f': // Find: find matching pattern
				findEntry(arg, &fileTree);
				break;
				
			case 'h': // Help: display options
				helpMsg();
				break;
				
			case 'm': // Move: move single entry
				moveDir(arg, &fileTree);
				break;
				
			case 'r': // Rename: rename single entry
				renameEntry(arg, &fileTree);
				break;
				
			default:
				printf("------------------------------------------"
				       "--------------------------------\n"
					   "Unrecognized command, enter 'h' or 'help' "
					   "for a list of accepted commands.\n"
					   "------------------------------------------"
				       "--------------------------------\n");
		}
		printf("%s:%s$ ", name, absPath);
	}
		
	destroyTree(&fileTree);
	
	return 0;
}