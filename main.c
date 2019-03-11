#include "player.h"
#include "render.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void print_player(PLAYER player, WINDOW* win)
{

    mvwprintw(win, 1, 1, "name: %s, health: %hu, items in inventory: %d", player.name, player.health, player.inv_count);
    for (int i = 0; i < player.inv_count; i++)
        mvwprintw(win, 2 + i, 1, "%d: %s\n", (player.inventory + i)->id, (player.inventory + i)->name);
}

int main(void)
{

    render_init();

    if (COLS != 174 || LINES != 49) {
        render_end();
        printf("Please resize your terminal to 49(lines)*173(collums)\n");
        return 1;
    }

    PLAYER* player = malloc(sizeof(PLAYER));
    if (player == NULL)
        return 1;
    srandom(time(NULL));
    player->inv_count = 0;
    player->health = random() % 100 + 1;
    player->inventory = NULL;

    WINDOW** container = init_cointainers();
    newdialog(player->name, "What is your name?");

    print_player(*player, container[1]);
    render_map(container[0]);

    mvprintw(LINES - 2, 1, "Press q to quit");
    refresh();
    wrefresh(container[0]);
    wrefresh(container[1]);

    int key, p_x = 10, p_y = 10;
    while ((key = getch()) != 'q') {

        switch (key) {
        case KEY_UP:
        case 'w':
            p_y--;
            break;
        case KEY_DOWN:
        case 's':
            p_y++;
            break;
        case KEY_LEFT:
        case 'a':
            p_x--;
            break;
        case KEY_RIGHT:
        case 'd':
            p_x++;
            break;
        }

        move(p_y, p_x);
        refresh();
    }

    render_end();

    /* free memory used by game, ncurses will leak memory, but its normal */
    for (int i = 0; i < player->inv_count; i++)
        free((player->inventory + i)->name);
    free(player->inventory);
    free(player);
}
