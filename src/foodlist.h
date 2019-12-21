#ifndef _SNAKE_H
	#include "snake.h"
#endif
#ifndef _STRING_H
	#include <string.h>
#endif
#ifndef __NCURSES_H
	#include <ncurses.h>
#endif

#define _FOODLIST_H 1
#define _FOOD_MAX 6

typedef struct point food_t;

typedef struct {
	food_t **list;
	uint8_t count;
	uint8_t max;
} foodlist_t;

foodlist_t *fl_new(uint8_t max);
void fl_free(foodlist_t *list);

bool fl_add(foodlist_t *fl, struct point coords);
void fl_rem(foodlist_t *fl, uint8_t index);

void fl_clear(foodlist_t *fl, WINDOW *window);

void fl_print(foodlist_t *fl, WINDOW *window);
