#include "render.h"

void render_init()
{

    initscr();
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);

        init_pair(10, COLOR_YELLOW, COLOR_BLACK);
        init_pair(11, COLOR_YELLOW, COLOR_RED);
        init_pair(12, COLOR_YELLOW, COLOR_GREEN);
        init_pair(13, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(14, COLOR_YELLOW, COLOR_BLUE);
        init_pair(15, COLOR_YELLOW, COLOR_MAGENTA);
        init_pair(16, COLOR_YELLOW, COLOR_CYAN);
        init_pair(17, COLOR_YELLOW, COLOR_WHITE);

        init_pair(20, COLOR_BLACK, COLOR_BLACK);
        init_pair(21, COLOR_RED, COLOR_RED);
        init_pair(22, COLOR_GREEN, COLOR_GREEN);
        init_pair(23, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(24, COLOR_BLUE, COLOR_BLUE);
        init_pair(25, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(26, COLOR_CYAN, COLOR_CYAN);
        init_pair(27, COLOR_WHITE, COLOR_WHITE);
    }
    keypad(stdscr, TRUE);
    noecho();
    refresh();
}

void render_end()
{

    endwin();
}

void render()
{
}

unsigned int _get_color_pair(char c)
{
    switch (c) {
    case 'x': /* black */
    default:
        return COLOR_PAIR(10);
    case 'r': /* red */
        return COLOR_PAIR(11);
    case 'g': /* green */
        return COLOR_PAIR(12);
    case 'y': /* yellow */
        return COLOR_PAIR(13);
    case 'b': /* blue */
        return COLOR_PAIR(14);
    case 'm': /* magenta */
        return COLOR_PAIR(15);
    case 'c': /* cyan */
        return COLOR_PAIR(16);
    case 'w': /* white */
        return COLOR_PAIR(17);

        /* blocked variants */
    case 'X': /* black */
        return COLOR_PAIR(20);
    case 'R': /* red */
        return COLOR_PAIR(21);
    case 'G': /* green */
        return COLOR_PAIR(22);
    case 'Y': /* yellow */
        return COLOR_PAIR(23);
    case 'B': /* blue */
        return COLOR_PAIR(24);
    case 'M': /* magenta */
        return COLOR_PAIR(25);
    case 'C': /* cyan */
        return COLOR_PAIR(26);
    case 'W': /* white */
        return COLOR_PAIR(27);
    }
}

void render_map(WINDOW* win)
{

    char* map_data = NULL;
    int cols = 0, rows = 0;

    const int RES = read_map(&map_data, &cols, &rows);
    if (RES == -1) {
        wprintw(win, "Error reading map (err -1)");
        return;
    }

    int nth_c = 0;
    char c = 0;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            c = map_data[nth_c++];
            char p = ' ';
            if (isupper(c))
                p = 'x';
            mvwaddch(win, y + 1, x + 1, p | _get_color_pair(c));
        }
    }

    free(map_data);
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
    for (int i = 0; i < 2; i++) {
        wattron(container[i], COLOR_PAIR(1));
        box(container[i], ACS_VLINE, ACS_HLINE);
        wattroff(container[i], COLOR_PAIR(1));
        wrefresh(container[i]);
    }

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
}
