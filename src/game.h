#ifndef _GAME_H
#define _GAME_H 1

#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 199309L
#endif
#include <stdbool.h>
#include <time.h>
#include "snake.h"
#include "foodlist.h"

#define _ESC 27
#define _SPACE 32
#define NANO_SECOND_MULTIPLIER 1000000U
#define _DELAY_HORIZON 45U * NANO_SECOND_MULTIPLIER
#define _DELAY_VERT 90U * NANO_SECOND_MULTIPLIER
#define _PAUSE_COUNTER_DELAY 1
#define _GAME_H 1

typedef struct {
	snake_t *snake;
	foodlist_t *food;
	uint32_t score;
	uint32_t death;
	WINDOW *game_screen;
	WINDOW *header;
} game_t;

game_t *game_new(WINDOW *header, WINDOW *game_screen);

void game_start(game_t *game);
void game_end(game_t *game);

#endif
