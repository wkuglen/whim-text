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
	long segment_number;
	char* string;
	bool dirty_bit;
	bool has_endline;
};

struct node
{
	struct line* data;
	struct node* next;
	struct node* previous;
};


// defined in whim.c
void moveBy (int, int);

// defined in manager.c
void initSimple (char* filename);
void setCharSimple (char, long, long);
void destroySimple ();
void readDumpIn ();

void init(char* filename);