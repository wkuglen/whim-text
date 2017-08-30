#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#ifndef MAX_WIN_COL
#define MAX_WIN_COL 80
#endif

extern int WIN_ROWS;
extern int WIN_COLS;// = 80;


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


extern int firstLineDisplayed;
extern int firstLineOffset;
extern int lastLineDisplayed;
extern int lastLineCutoff;

bool init (char* filename);
void setChar (char, struct line*, int);
void addChar (char, struct line*, int);

void scrollUp ();
void scrollDown ();
