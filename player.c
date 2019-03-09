#include "player.h"

int add_to_inventory(PLAYER *player, ITEM *item) {

	ITEM *tp = realloc(player->inventory, (player->inv_count+1)*sizeof(ITEM));
	if (tp == NULL)
		return 0;
	player->inventory = tp;
	*(player->inventory + player->inv_count) = *item;
	player->inv_count++;
	return 1;
}

int remove_from_inventory(PLAYER* player, short id) {
	return 0;
}
