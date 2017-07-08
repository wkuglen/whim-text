#include <curses.h> // required

int r,c, // current row and column (upper-left is (0,0))
    nrows, // number of rows in window
    ncols; // number of columns in window

void draw(char dc) {
	move (r,c);
	delch(); insch(dc);
	refresh();
	r++;
	if (r == nrows) {
		r = 0;
		c++;
		if (c == ncols) c = 0;
	}
}

main () {
	int i; char d;
	WINDOW *wnd;

	wnd = initscr();
	cbreak();
	noecho();
	getmaxyx (wnd, nrows, ncols); 
	clear();
	refresh();

	r = 0; c = 0;
	while (1) {
		d = getch();
		if (d == 'q') break;
		draw (d);
	}

	endwin();
}
