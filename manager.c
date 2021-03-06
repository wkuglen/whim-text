#include "whim.h"

struct node* list;
struct node* topNode;
struct node* bottomNode;

struct line* *array;
long maxArraySize, arraySize;

FILE* openFile;

char* arraySimple;
long sizeSimple = 0;
long displacedSimple = 0;

// extern int WIN_ROWS;
// extern int WIN_COLS;

int firstLineDisplayed;
int firstLineOffset;
int lastLineDisplayed;
int lastLineCutoff;

int test () {
	int c;
	int out = 0;
	while ((c = fgetc(openFile)) != EOF)
	{
		printf ("%x ", c);
  		out++;
	}
	if (feof(openFile))
	{
	  // hit end of file
		perror ("eof");
		return out;
	}
	else
	{
	  // some other error interrupted the read
		return -1;
	}
}
bool initSimple (char* filename) {
	// do something with filename
	openFile = fopen (filename, "r+");

	if (openFile == NULL) {
		openFile = fopen (filename, "w+");
		if (openFile == NULL) {
			printf ("FAIL: %s cannot be found or created.\n", filename);
			return false;
		} else {
			fclose (openFile);
			openFile = fopen (filename, "r+");
			if (openFile == NULL) {
				printf("FAIL: There was a problem in creating %s\n", filename);
				return false;
			}
		}
	}
	// setup array
	sizeSimple = 2 * WIN_COLS * WIN_ROWS;
	// printf("%d %d %d\n", WIN_COLS, WIN_ROWS, sizeSimple );
	arraySimple = malloc (2 * WIN_COLS * WIN_ROWS * sizeof(char));

	test();
	return true;
	//fseek (openFile, 0, 0);
	//printf ("%d\n", test());//fread (arraySimple, sizeof(char), sizeof(arraySimple), openFile));
	//if (feof(openFile)) perror ("eof");
	//if (ferror(openFile)) perror("error");
}

void readDumpIn () {
	int c;
	int out = 0;

	fseek (openFile, 0, 0);
	while ((c = fgetc(openFile)) != EOF)
	{
		//printf ("%c", c);
		arraySimple[out] = c;
		addch(c);
  		out++;
	}

	// size_t bytesRead = fread (arraySimple, sizeof(char), sizeSimple, openFile);
	// for (int i = 0; i < bytesRead; ++i)
	// {
	// 	addch(arraySimple[i]);
	// 	moveBy (0, 1);
	// }
	refresh ();
}

void setCharSimple (char c, long row, long col) {
	arraySimple[row * WIN_COLS + col] = c;
	fseek (openFile, row * WIN_COLS + col, displacedSimple);
	fputc (c, openFile);
}

char getCharSimple (long row, long col) {
	return arraySimple[row * WIN_COLS + col];
}

//slide right
// temp = *current
//repeat: until end of array or null char?
// current++
// temp2 = *current
// *current = temp
// temp = temp2

void destroySimple () {
	fseek (openFile, 0, SEEK_END);
	fputc ('\n', openFile);
	fclose (openFile);
	printf("%s\n", arraySimple);
	free (arraySimple);
}

// void addLine (struct node*, struct line*);
bool fileToLine (struct line* newline);
void reallocate (struct line*);
void addLine (long, struct line*);
void shiftUp (long);
void shiftDown (long);


/*
 * Writing into memory/disk:
 *  As things are pushed off the top write them to temp.
    then you continue to write in order.
 *  As things are pushed off the bottom, goto eof then find first
    \n that is not immediately ahead of eof. Write the last line 
    to a seperate temp but backwards. read into save file backwards 
    as well
 *  Have a recovery file that is fully updated for every 30 lines 
    that are pushed off
 */


bool init (char* filename) {
	//do something with filename

	openFile = fopen (filename, "r+");

	if (openFile == NULL) {
		openFile = fopen (filename, "w+");
		if (openFile == NULL) {
			printf ("FAIL: %s cannot be found or created.\n", filename);
			return false;
		} else {
			fclose (openFile);
			openFile = fopen (filename, "r+");
			if (openFile == NULL) {
				printf("FAIL: There was a problem in creating %s\n", filename);
				return false;
			}
		}
	}
	//set up linked list
	//list = topNode = bottomNode = NULL;
	//read line by line
	// printf("file opened.\n");
	array = malloc (2 * WIN_ROWS * sizeof(struct line*));
	maxArraySize = 2 * WIN_ROWS;

	int lineCount = 0;
	bool notEOF = true;
	while ((lineCount < maxArraySize) && (notEOF)) {
		struct line* nextLine = malloc (sizeof (struct line));
		// printf("nextline ");
		notEOF = fileToLine (nextLine);
		array[lineCount] = nextLine;// addLine (lineCount, nextLine);
		++lineCount;
	}

	arraySize = lineCount;
	int totalLines = lineCount;
	int i = 0;
	// lineCount = 0;
	// while (lineCount < WIN_COLS && i < totalLines)
	// {
	// 	int length = strlen (array[i]->string);
	// 	mvaddnstr (lineCount, 0, array[i]->string, (WIN_ROWS - lineCount)*WIN_COLS);
	// 	lineCount += (int) (length / WIN_COLS);
	// 	if (length % WIN_COLS != 0)
	// 		lineCount++;
	// 	i++;
	// }

	//offset = 0
	//line = 0
	//for every row
	// mvaddnstr (i, 0, array[line]->string[offset], WIN_COLS)
	// if strlen (array[line]->string[offset]) > WIN_COLS
	//		offset += win_cols
	// else
	// 		line++;
	//		offset = 0;
	int offset = 0;
	lineCount = 0;
	for (i = 1; i < WIN_ROWS && i < totalLines; i++)
	{
		mvaddnstr (i, 0, &array[lineCount]->string[offset], WIN_COLS);
		if (strlen (&array[lineCount]->string[offset]) > WIN_COLS)
			offset += WIN_COLS;
		else {
			lineCount++;
			offset = 0;
		}
		refresh();
	}

	if (offset <= 0) {
		lastLineDisplayed = lineCount - 1;
		lastLineCutoff = array[lineCount - 1]->last_char;
	} else {
		lastLineDisplayed = lineCount;
		lastLineCutoff = offset;
	}

	return true;
}

//

bool fileToLine (struct line* newline) {
	char c;
	int i = 0;
	
	// newline = malloc (sizeof (struct line*));
	newline->string = malloc (80 * sizeof (char));
	newline->allocated = 80;
	newline->last_char = -1;

	while ((c = fgetc(openFile)) != EOF)
	{
		newline->string[i] = c;
		//addch(c);
		if(c == '\n') {
			newline->last_char++;
			break;
		}
  		i++;
  		newline->last_char++;
  		if (i >= ((float)newline->allocated) * 0.75)
  			reallocate (newline);
	}
	if (feof(openFile))
		return false;
	return true;
}

//Replace a char
void setChar (char c, struct line* line, int position) {
	if (c == '\n') { /* do something */}

	if (position > line->last_char)
		return;

	line->string[position] = c;
}

//Add or insert a char
void addChar (char c, struct line* line, int position) {
	//check position validity
	if (c == '\n') { /* do something */}

	if (position > line->last_char+1)
		position = line->last_char+1;

	int i;
	for (int i = line->last_char + 1; i > position; --i)
	{
		line->string[i] = line->string[i - 1];
	}
	line->string[position] = c;
	line->last_char++;
	if (line->last_char >= ((float) line->allocated) * 3 / 4)
		reallocate (line);

}

void reallocate (struct line* l) {
	char* newAllocation = malloc (2 * l->allocated);
	for (int i = 0; i <= l->last_char; ++i)
	{
		newAllocation[i] = l->string[i];
	}
	free (l->string);
	l->string = newAllocation;
	l->allocated = 2 * l->allocated;
}
// void addLine (struct node* above, struct line* nextLine) {
// 	struct node* insert = malloc (sizeof (struct node));
// 	insert->data = nextLine;

// 	if (above == NULL)
// 	{
// 		insert->previous = NULL;
// 		insert->next = list;
// 		list = insert;
// 	} else {
// 	//Get all nodes
// 	struct node* below = above->next;
	

// 	//insert into linked list
// 	above->next = insert;
// 	insert->previous = above;
// 	insert->next = below;
// 	below->previous = insert;
// 	}
// }

void addLine (long insertAt, struct line* nextLine) {
	printf ("addline");
	if (insertAt == maxArraySize) {
		//shift up
		shiftUp (insertAt - 1);
		array[insertAt - 1] = nextLine;
	} else if (insertAt == 0) {
		//shift down
		shiftDown (insertAt);
		array[insertAt] = nextLine;
	} else {
		if (insertAt <= maxArraySize / 2) {
			//in first half
			//spatially close to top so shift down
			shiftUp (insertAt);
		} else {
			//in second half
			//spatially close to bottom so shift up
			shiftDown (insertAt);
		}

		array[insertAt] = nextLine;
	}
}

// input 2
// [0 1 2 3 4]
// [1 2 2 3 4]

// input 4
// [0 1 2 3 4]
// [1 2 3 4 4]
void shiftUp (long exclusiveStart) {
	long i;
	struct line* top = array[0];

	if (exclusiveStart != 0) {
		for (i = 0; i < exclusiveStart; ++i) {
			array[i] = array[i + 1];
		}
	}
	if (top != NULL)
		free (top);
}


// input 2
// [0 1 2 3 4]
// [0 1 1 2 3]

// input 0
// [0 1 2 3 4]
// [0 0 1 2 3]
void shiftDown (long exclusiveStart) {
	long i;
	struct line* bottom = array[WIN_ROWS - 1];

	if (exclusiveStart != WIN_ROWS - 1) {
		for (i = WIN_ROWS - 1; i > exclusiveStart; --i) {
			array[i] = array[i - 1];
		}
	}
	if (bottom != NULL)
		free (bottom);
}

void scrollUp () {
	if (firstLineDisplayed == 0 && firstLineOffset == 0) {
		//either top of array or top of file
		//need a first line loaded and last line loaded variable
		// if top of array
		//   shiftDown
		// if top of file
		//   beep() & STOP
		beep ();
	} else {
		if (firstLineDisplayed == 0 && firstLineOffset != 0) {
			scrl (-1);
			// calculate new offset (must be >= 0);
			firstLineOffset -= WIN_COLS;
			if (firstLineOffset < 0) firstLineOffset = 0;

			// add characters to line from offset to offset + WIN_COLS
			addstr (&(array[0]->string[firstLineOffset]));
		} else if (firstLineDisplayed != 0 && firstLineOffset != 0) {
			scrl (-1);
			// calculate new offset (must be >= 0);
			// add characters to line from offset to offset + WIN_COLS
			firstLineOffset -= WIN_COLS;
			if (firstLineOffset < 0) firstLineOffset = 0;

			// add characters to line from offset to offset + WIN_COLS
			addstr (&(array[firstLineDisplayed]->string[firstLineOffset]));
		} else {
			scrl (-1);
			// firstLineDisplayed--
			firstLineDisplayed--;
			//if last_char > WIN_COLS 
			// add characters to line from last_char - last_char%WIN_COLS to last_char
			// set offset to last_char - last_char%WIN_COLS
			if (array[firstLineDisplayed]->last_char >= WIN_COLS) {
				int lc = array[firstLineDisplayed]->last_char;
				firstLineOffset = lc - (lc % WIN_COLS);
				addstr (&(array[firstLineDisplayed]->string[firstLineOffset]));
			} else {
				//else
				// add characters from line
				addstr (array[firstLineDisplayed]->string);
			}
		}

		//now adjust last line numbers
		//if lastLineCutoff = 0, lastLineDisplayed--
		//Cutoff -= WIN_COLS (must be >= 0)

		if (lastLineCutoff == 0) {
			lastLineDisplayed--;
		}
		lastLineCutoff -= WIN_COLS;
		if (lastLineCutoff < 0) lastLineCutoff = 0;
		refresh ();
	}
}


void scrollDown () {
	if (lastLineDisplayed >= arraySize -1 && lastLineCutoff >= array[lastLineDisplayed]->last_char) {
		beep();
	} else {
		scrl (1);
		if (lastLineDisplayed == arraySize-1 && lastLineCutoff < array[lastLineDisplayed]->last_char) {
			mvaddstr (WIN_ROWS-2, 0, &(array[lastLineDisplayed]->string[lastLineCutoff]));
			lastLineCutoff += WIN_COLS;
		} else if (lastLineCutoff >= array[lastLineDisplayed]->last_char) {
			lastLineDisplayed++;
			lastLineCutoff = WIN_COLS;
			mvaddstr (WIN_ROWS-2, 0, array[lastLineDisplayed]->string);
		} else {
			mvaddstr (WIN_ROWS-2, 0, &(array[lastLineDisplayed]->string[lastLineCutoff]));
			lastLineCutoff += WIN_COLS;
		}

		if (array[firstLineDisplayed]->last_char - firstLineOffset < WIN_COLS) {
			firstLineDisplayed++;
			firstLineOffset = 0;
		} else {
			firstLineOffset += WIN_COLS;
		}
		refresh ();

	}
}
