#include "render.h"

void render_init()
{

    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_WHITE);
        init_pair(5, COLOR_GREEN, COLOR_GREEN);
        init_pair(6, COLOR_BLUE, COLOR_BLUE);
        init_pair(7, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(8, COLOR_RED, COLOR_RED);
    }
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    refresh();
}

void render_end()
{

    endwin();
}

void render()
{
}

void render_map(WINDOW* win)
{

#include "maps.h"

    const int STRECH = 2;
    unsigned int option = COLOR_PAIR(1);
    int RENDER_COL = 0;
    for (int COL = 0; COL < MAP_COLS; COL++) {
        for (int t = 0; t < STRECH; t++) {
            for (int LINE = 0; LINE < MAP_LINES; LINE++) {
                switch (MAP_ONE[LINE][COL]) {
                case 'b':
                    option = COLOR_PAIR(4);
                    break;
                case 'f':
                    option = COLOR_PAIR(5);
                    break;
                case 'w':
                    option = COLOR_PAIR(6);
                    break;
                case 'x':
                    option = COLOR_PAIR(7);
                    break;
                case 'm':
                    option = COLOR_PAIR(8);
                    break;
                }

                mvwaddch(win, LINE + 1, RENDER_COL + 1, MAP_ONE[LINE][COL] | option);
            }
            RENDER_COL++;
        }
    }
}

WINDOW** init_cointainers()
{

    /* set the background of stdscr to white */
    /* makes it look better on terminals with odd height */
    /* bkgd(COLOR_PAIR(1)); */
    /* refresh(); */

    int h, w;
    getmaxyx(stdscr, h, w);

    static WINDOW* container[2];
    /* split the terminal window to 3/4 and 1/4 */
    const int three_quaters = h / 4 * 3;
    container[0] = newwin(three_quaters, w, 0, 0);
    container[1] = newwin(h - three_quaters, w, three_quaters, 0);
    /* draw the oulines */
    wattron(container[0], COLOR_PAIR(1));
    box(container[0], ACS_VLINE, ACS_HLINE);
    wattroff(container[0], COLOR_PAIR(1));
    wattron(container[1], COLOR_PAIR(1));
    box(container[1], ACS_VLINE, ACS_HLINE);
    wattroff(container[1], COLOR_PAIR(1));
    wprintw(container[0], "%d COLS, %d LINES", w, h);
    wrefresh(container[0]);
    wrefresh(container[1]);

    return container;
}

WINDOW* newwin_center(int y, int x)
{
    int h, w;
    getmaxyx(stdscr, h, w);
    /* new window positioned in the center of the screen */
    return newwin(y, x, h / 2 - y / 2 - 1, w / 2 - x / 2 - 1);
}

void newdialog(char* buffer, /* shold be allocated to atleast the withd of the dialog -> W */
    const char* message)
{

    /* width of the dialog */
    const int W = 50;

    curs_set(1); /* disable cursor */
    WINDOW* win = newwin_center(3, W);
    wattron(win, COLOR_PAIR(3));
    box(win, ACS_VLINE, ACS_HLINE);
    wattroff(win, COLOR_PAIR(3));
    touchwin(win);
    /* print message with white space around */
    mvwaddch(win, 0, 1, ' ');
    mvwprintw(win, 0, 2, message);
    mvwaddch(win, 0, strlen(message) + 2, ' ');
    /* move the cursor to the input field */
    wmove(win, 1, 1);

    wrefresh(win);

    int x = 1, ch;

    while ((ch = getch()) != 10) {
        /* is c printable */
        if (isprint(ch)) {
            if (x + 1 != W) { /* if cursor not at the end */
                mvwaddch(win, 1, x++, ch);
                if (x + 1 == W)
                    wmove(win, 1, --x); /* move one back when the cursor is at VLINE */
            }
        } else {
            switch (ch) {
            case KEY_LEFT:
                if (x - 1 != 0)
                    x--;
                break;
            case KEY_RIGHT:
            case ' ':
                if (x != W - 2)
                    x++;
                break;
            /* backspace is different in terminal emulators and tty */
            case 127:
            case '\b':
            case KEY_BACKSPACE:
                if (x == W - 2 && mvwinch(win, 1, x) != ' ')
                    mvwaddch(win, 1, x, ' ');
                else if (x - 1 != 0)
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
    for (int i = 1; i < W - 1; i++)
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
