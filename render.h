#ifndef RENDER_H
#define RENDER_H

#include "utils.h"
#include "const.h"
#include <ctype.h>
#include <ncurses.h>
#include <string.h>

void render_init();
void render_end();
void render();
WINDOW* newwin_center(int y, int x);
WINDOW** init_cointainers();
void newdialog(char* buffer, const char* message);
void render_map(WINDOW* win);
#endif
