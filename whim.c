#include <curses.h>

int r, c, nrows, ncols;
bool replace;

//void insert_mode ();
//printf "\x1b[1J \x1b[H"; sleep 2; 
//printf "So this is how it begins...\n"; sleep 2; 
//printf "\x1b[1J \x1b[H"

void draw (int d) {
	move (r,c);
	addch(d); //delch (); insch (d);
	
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
	r++;
	c = 0;
	move (r, c);
	refresh ();

}

void moveBy (int rows, int cols) {
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
	refresh ();
}

int main(int argc, char const *argv[])
{
	int i;
	int d;
	WINDOW *wnd;

	wnd = initscr(); //curses > init window
	cbreak(); //curses > raw mode (chars sent to program)
	noecho(); //curses > no echoing
	getmaxyx(wnd, nrows, ncols); //curses > get size of window
	clear (); //curses > clear screen
	refresh ();
	keypad (wnd, TRUE);

	r = 0; c = 0;
	while (1) {
		d = getch (); //curses > input from keyboard
		if (d == '\x1b') break;
		switch (d) {
			case KEY_DOWN: moveBy (1, 0); break;
			case KEY_UP: moveBy (-1, 0); break;
			case KEY_LEFT: moveBy (0, -1); break;
			case KEY_RIGHT: moveBy (0, 1); break;
			case KEY_SLEFT: moveBy (0, -ncols); break;
			
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
	return 0;
}