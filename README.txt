*******************************************************************************
Shawn Hustins                                                           0884015
CIS 2520 DE                                                    A3: File Manager
Aug 5 2016
*******************************************************************************


***********
Compilation
***********

make targets:
	all: builds test and program
	
	test: builds bin/treeTest
	
	program: builds bin/fileManager
	
	clean: removes build artifacts (executables and objects)


**********************
Running the program(s)
**********************

$ ./bin/treeTest
	Runs a test of the AVL tree library. Testing information can be found in
	docs/
	
$ ./bin/fileManager {filePath}
	Runs the file manager application. filePath should point to a directory
	whose contents and subdirectories will be accessed in program use.
	The program is made to somewhat emulate a shell. Each user input is prefixed
	with the user name and current absolute location in directories.
	
	*********************************************
	Example demonstration of directory traversal:
	*********************************************
	
	$ ./bin/fileManager .
	------------------------------------
	CLI File Manager: Enter 'h' for help
	------------------------------------
	shawn:/home/shawn/CIS2520/A3$ find */
	---------------------------------------
	Searching for '*/' in '.'
	---------------------------------------
	-- ./
	-- ./bin/
	-- ./docs/
	-- ./docs/extraTestDir/
	-- ./include/
	-- ./obj/
	-- ./src/
	Matches: 7
	---------------------------------------
	shawn:/home/shawn/CIS2520/A3$ move docs/
	shawn:/home/shawn/CIS2520/A3/docs$ find *
	---------------------------------------
	Searching for '*' in './docs/'
	---------------------------------------
	-- ./docs/
	-- ./docs/AVLTree.txt
	-- ./docs/extraTestDir/
	Matches: 3
	---------------------------------------
	shawn:/home/shawn/CIS2520/A3/docs$ move ..
	shawn:/home/shawn/CIS2520/A3$ find *.c
	---------------------------------------
	Searching for '*.c' in './'
	---------------------------------------
	-- ./src/AVLTree.c
	-- ./src/fileFunctions.c
	-- ./src/fileManager.c
	-- ./src/test.c
	Matches: 4
	---------------------------------------
	shawn:/home/shawn/CIS2520/A3$ q
	
	END
	*********************************************
	
fileManager Program Commands:

	'd'/'delete' : delete the file located at the relative file path Cannot 
	               delete file directories or multiple files at once.
				   $ delete {filePath}
				   
	'f'/'find'   : list matches to the given expression. If just a filePath
	               is specified, an efficient search for the file will be
	               performed. If a wildcard (or two) are specified, a complete
	               search for matches will be performed.
	               $ find {file}      -- example "src/file.c" (just this file)
	               $ find *{postfix}  -- example "*.c" (every .c file)
	               $ find *{infix}*   -- example "*test*" (anything named test)
	               $ find {prefix}*	  -- example "src/*" (everything in src/)
	               $ find *
				   
	'h'/'help'   : display all program options
	               $ help

	'm'/'move'   : move to another directory. Can move down multiple directories
	               at a time but can only move up a directory one at a time
				   $ move {folder}    -- example "A3/src/"
				   $ move ..

	'q'/'quit'   : quit application
	               $ q

	'r'/'rename' : rename a file. Cannot rename or move directories. Can move
                   between directories.
				   $ rename {file} {new name} -- example "src/test.c test.c"
                                              -- example "test.c src/test/c"
                                              -- example "src/test.c new.c"

*******
Testing
*******

	Testing documents of AVL tree library can be found in docs/
	
	The file manager program, as it is user driven, does not have any automated
	testing. It was however, thoroughly tested on many different directories of
	many different sizes. File manipulation was tested for a wide range of file
	types and file permissions. Generally, any rename operation on an 
	inaccesible file, will leave it on the drive as well as in the programs file
	tree.
	
	Every command was also ran with unexpected arguments to ensure that they
	were handled gracefully. ex. "$ rename src/test.c test/" has no effect and
	alerts the user "Cannot move directories."
	
	As an added note, I implemented two versions of the find functionality so
	that I could still demonstrate the quick searching abilities of the AVL tree
	(since finding substrings requires a complete search).

*****************
Known Limitations
*****************

	Some are mentioned in program commands above.

	An attempt to navigate up a directory from the the initial directory then
	back down from there will sometimes print the absolute file path incorrectly

**********
References
**********

	A3 Spec

	Found in include/dbg.h:
		Shaw, Zed (2015) Zed's Awesome Debug Macros [source code] Addison-Wesley


*************************
Academic Integrity Pledge
*************************

I have exclusive control over this submission via my password.
By including this file with my submitted files, I certify that:

1) I have read and understood the University policy on academic integrity;
2) I have completed the Computing with Integrity Tutorial on Moodle; and
3) I have achieved at least 80% in the Computing with Integrity Self Test.

I assert that this work is my own.

I have appropriately acknowledged any and all material (data, images, ideas or
words) that I have used, whether directly quoted or paraphrased.  Furthermore,
I certify that this assignment was prepared by me specifically for this course.
