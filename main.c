#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "player.h"
#include "render.h"

void print_player(PLAYER player){

	printf("Name: %s\nHealth: %hu\nInv_count: %d\n", player.name, player.health, player.inv_count);
	for(int i = 0; i < player.inv_count; i++) {
	
		printf("%d: %s\n", (player.inventory + i)->id, (player.inventory + i)->name);
	
	}
}

int main(void) {

	PLAYER *player = malloc(sizeof(PLAYER));
	if (player == NULL)
		return 1;
	srandom(time(NULL));
	player->inv_count = 0;
	player->health = random() % 100 + 1;
	player->inventory = NULL;
	
	render_init();
	
	newdialog(player->name, "What is your name?");
	
	render_end();
	
	print_player(*player);

	/* free memory used by game, ncurses will leak memory, but its normal */
	for(int i = 0; i<player->inv_count; i++)
		free((player->inventory + i)->name);
	free(player->inventory);
	free(player);

}
