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
void initSimple (char* filename) {
	// do something with filename
	openFile = fopen (filename, "r+");

	if (openFile == NULL) {
		printf ("FAIL");
	}
	// setup array
	sizeSimple = 2 * WIN_COLS * WIN_ROWS;
	printf("%d %d %d\n", WIN_COLS, WIN_ROWS, sizeSimple );
	arraySimple = malloc (2 * WIN_COLS * WIN_ROWS * sizeof(char));

	test();
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
void addLine (long, struct line*);
void shiftUp (long);
void shiftDown (long);

void init (char* filename) {
	//do something with filename

	//set up linked list
	//list = topNode = bottomNode = NULL;
	//read line by line
	array = malloc (2 * WIN_ROWS * sizeof(struct line*));
	maxArraySize = 2 * WIN_ROWS;

	int lineCount = 0;
	while (lineCount < maxArraySize) {
		struct line* nextLine = malloc (sizeof (struct line));
		addLine (lineCount, nextLine);
		++lineCount;
	}
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

	free (bottom);
}


