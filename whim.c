#include <curses.h>
#include <stdio.h>
#include <strings.h>
#include "whim.h"

int r, c, nrows, ncols;
bool replace;

//void insert_mode ();
//printf "\x1b[1J \x1b[H"; sleep 2; 
//printf "So this is how it begins...\n"; sleep 2; 
//printf "\x1b[1J \x1b[H"

void draw (int d) {
	move (r,c);
	addch(d); //delch (); insch (d);
	
	setCharSimple (d, r, c);
	c++;
	if (c == ncols) {
		c = 0;
		r++;
		if (r == nrows) r = 0;
	}
	// move (r, c);
	refresh();
}

void delete () {

	if (c != 0) {
		c--;
	} else {
		if (r == 0) beep();
		r--;
		if (r < 0) r = 0;
		else c = ncols;
	}
	move (r, c);
	delch();
	refresh ();
}

void carriageReturn () {
	if (r > nrows) beep();
	addch ('\n');//delch(); insch ('\n');
	setCharSimple(0x0a, r, c);
	r++;
	c = 0;
	move (r, c);
	refresh ();

}

void moveBy (WINDOW *wnd, int rows, int cols) {
	//keypad (wnd, FALSE);
	if (r+rows > nrows)
		r = nrows;
	else if (r+rows < 0)
		r = 0;
	else 
		r += rows;

	if (c+cols > ncols)
		c = ncols;
	else if (c+cols < 0)
		c = 0;
	else
		c += cols;

	move (r, c);

	// char s = getCharSimple (r, c);
	// if (s < 32)
	// 	moveBy (wnd, 0, -1);
	
	// move (80, 80);

	// printw("%x", s);
	// refresh ();
	// move (r, c);
	refresh ();
	//keypad (wnd, TRUE);
}


//need to look up
// fseek(), fputc(), fscanf(), fread()
int main(int argc, char const *argv[])
{
	int i;
	int d;
	WINDOW *wnd;

	if (argc == 2) {
		initSimple (argv[1]);
	} else {
		initSimple ("will.txt");
	}

	wnd = initscr(); //curses > init window
	cbreak(); //curses > raw mode (chars sent to program)
	noecho(); //curses > no echoing
	getmaxyx(wnd, nrows, ncols); //curses > get size of window
	if (ncols > MAX_WIN_COL) ncols = MAX_WIN_COL;
	clear (); //curses > clear screen
	refresh ();
	keypad (wnd, TRUE);

	readDumpIn ();
	r = 0; c = 0;
	move(r, c);
	refresh();
	while (1) {
		d = getch (); //curses > input from keyboard
		if (d == '\x1b') break;
		switch (d) {
			case KEY_DOWN: moveBy (wnd, 1, 0); break;
			case KEY_UP: moveBy (wnd, -1, 0); break;
			case KEY_LEFT: moveBy (wnd, 0, -1); break;
			case KEY_RIGHT: moveBy (wnd, 0, 1); break;
			case KEY_SLEFT: moveBy (wnd, 0, -ncols); break;
			
			case KEY_ENTER:
			case 13:
			case 10: carriageReturn (); break;

			case 8:
			case 127:
			case KEY_BACKSPACE: delete(); break;

			default: draw (d);
		}
		// if (d == KEY_LEFT)
		// 	printw ("left");
		// else if (d == '\x1b' || d == KEY_CODE_YES) {
		// 	printw ("%c", d); refresh();
		// 	if (d == KEY_DOWN) moveBy (1, 0);
		// 	else if (d == KEY_UP) moveBy (-1, 0);
		// 	else if (d == KEY_LEFT) moveBy (0, -1);
		// 	else if (d == KEY_RIGHT) moveBy (0, 1);
		// 	else break;
		// }
		// else {
		// 	if (d == 13 || d == 10 || d == KEY_ENTER) carriageReturn ();
		// 	// if (d == '\x1b') break;
		// 	// if (d == 13 || d == 10) carriageReturn ();
		// 	// else if (d == 127 || d == 8) delete();
		// 	else draw (d);
		// }
	}

	endwin();
	destroySimple();
	return 0;
}