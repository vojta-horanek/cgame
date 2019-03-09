#ifndef RENDER_H
#define RENDER_H

#include <ncurses.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

void render_init();
void render_end();
void render();
WINDOW* newwin_center(int y, int x);
void newdialog(char *buffer, const char *message);

#endif
