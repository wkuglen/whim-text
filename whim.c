#include <curses.h>

int r, c, nrows, ncols;

//void insert_mode ();
//printf "\x1b[1J \x1b[H"; sleep 2; 
//printf "So this is how it begins...\n"; sleep 2; 
//printf "\x1b[1J \x1b[H"

void draw (char d) {
	move (r,c);
	delch (); 
	insch (d);
	
	c++;
	if (c == ncols) {
		c = 0;
		r++;
		if (r == nrows) r = 0;
	}
	move (r, c);
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
	delch();
	insch ('\n');
	r++;
	c = 0;
	move (r, c);
	refresh ();

}

int main(int argc, char const *argv[])
{
	int i;
	char d;
	WINDOW *wnd;

	wnd = initscr(); //curses > init window
	cbreak(); //curses > raw mode (chars sent to program)
	noecho(); //curses > no echoing
	getmaxyx(wnd, nrows, ncols); //curses > get size of window
	clear (); //curses > clear screen
	refresh ();

	r = 0; c = 0;
	while (1) {
		d = getch (); //curses > input from keyboard
		if (d == '\x1b') break;
		if (d == 13 || d == 10) carriageReturn ();
		else if (d == 127 || d == 8) delete();
		else draw (d);
	}

	endwin();
	return 0;
}