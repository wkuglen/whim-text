#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#ifndef MAX_WIN_COL
#define MAX_WIN_COL 80
#endif

static int WIN_ROWS = 80;
static int WIN_COLS = 80;


struct line
{
	// long segment_number;
	char* string;
	int allocated;
	int last_char;
	//bool dirty_bit;
	//bool has_endline;

};

struct node
{
	struct line* data;
	struct node* next;
	struct node* previous;
};


// defined in whim.c
void moveBy (WINDOW*, int, int);

// defined in manager.c
bool initSimple (char* filename);
void setCharSimple (char, long, long);
char getCharSimple (long, long);
void destroySimple ();
void readDumpIn ();


static int firstLineDisplayed = 0;
static int firstLineOffset = 0;
static int lastLineDisplayed = 0;
static int lastLineCutoff = 0;

bool init (char* filename);
void setChar (char, struct line*, int);
void addChar (char, struct line*, int);
