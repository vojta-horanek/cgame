#include "render.h"

void render_init() {
	
	initscr();
	
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	refresh();
}

void render_end() {
	
	endwin();
}

void render() {
	
}

WINDOW* newwin_center(int y, int x) { 
	int h, w; 
	getmaxyx(stdscr, h, w); 
	/* new window positioned in the center of the screen */
	return newwin(y, x, h/2-y/2-1, w/2-x/2-1);
}

void newdialog(char *buffer, /* shold be allocated to atleast the withd of the dialog -> W */ 
				const char *message) {

	/* width of the dialog */
	const int W = 50;

	curs_set(1); /* disable cursor */
	WINDOW *win = newwin_center(3, W);
	box(win, ACS_VLINE, ACS_HLINE);
    touchwin(win);
    /* print message with white space around */
    mvwaddch(win, 0, 1, ' ');
    mvwprintw(win, 0, 2, message);
    mvwaddch(win, 0, strlen(message) + 2, ' ');
    /* move the cursor to the input field */
    wmove(win, 1, 1);
	
	wrefresh(win);
    
    int x = 1, ch;
    
    while((ch = getch()) != 10) {
		/* is c printable */
		if(isprint(ch)) {
			if(mvwinch(win, 1, x + 1) != ACS_VLINE) /* if cursor not at the end */
				mvwaddch(win, 1, x++, ch);
			else
				wmove(win, 1, x); /* move one back when the cursor is at VLINE */
		} else {
			switch (ch)
			{
				case KEY_LEFT:
					if(mvwinch(win, 1, x - 1) != ACS_VLINE)
						x--;
					break;
				case KEY_RIGHT:
				case ' ':
					if(mvwinch(win, 1, x + 1) != ACS_VLINE)
						x++;
					break;
				/* backspace is different in terminal emulators and tty?*/
				case 127:
				case '\b':
				case KEY_BACKSPACE:
					if(mvwinch(win, 1, x - 1) != ACS_VLINE)
					mvwaddch(win, 1, --x, ' '); /* not using mvwdelch() since it breaks the border */
					break;
			
			}
			wmove(win, 1, x);
        }

        wrefresh(win);
    }
    
	char tmp_buffer[W];
	int size = 0;
	
	/* go trought each char in the input field */
	for(int i = 1; i < W-2; i++)
		/* store entered char at px = i in the buffer */
		tmp_buffer[size++] = mvwinch(win, 1, i);
	tmp_buffer[size++] = '\0';
	
	trimwhitespace(buffer, size, tmp_buffer);
	
	/* remove dialog from screen and memory */
    wclear(win);
	wrefresh(win);
	delwin(win);
	/* move to the begining and disable cursor */
	move(0, 0);
	curs_set(0);
	
}

