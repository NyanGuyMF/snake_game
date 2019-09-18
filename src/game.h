#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 199309L
#endif
#ifndef _SNAKE_H
	#include "snake.h"
#endif
#ifndef _FOODLIST_H
	#include "foodlist.h"
#endif
#ifndef _STDBOOL_H
	#include <stdbool.h>
#endif
#ifndef _TIME_H
	#include <time.h>
#endif

#define _ESC 27
#define _SPACE 32
#define NANO_SECOND_MULTIPLIER 1000000U
#define _DELAY_HORIZON 115U * NANO_SECOND_MULTIPLIER
#define _DELAY_VERT 170U * NANO_SECOND_MULTIPLIER
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
