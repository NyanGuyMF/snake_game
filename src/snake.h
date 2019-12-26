#ifndef _SNAKE_H
#define _SNAKE_H 1

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>	/* strlen(1) */

#define _SNAKE_DEFAULT_LEN 3
#define _SNAKE_GROW_LEN 2
#define _SNAKE_H 1
#define point_equals(o, a) ((o)->x == (a)->x) && ((o)->y == (a)->y)

typedef enum {
	LEFT = 0,
	UP,
	RIGHT,
	DOWN,
} direction_t;

typedef struct snake_body snake_body_t;

struct point {
	uint16_t x;
	uint16_t y;
};

struct snake_body {
	struct point coords;
	snake_body_t *next;
};

typedef struct {
	snake_body_t *head;
	snake_body_t *tail;
	direction_t direction;
	unsigned int is_alive : 1;
	uint16_t len;
} snake_t;

snake_t *snake_new(uint16_t initial_len, uint16_t head_x, uint16_t head_y);
snake_t *snake_new_centered(uint16_t initial_len, WINDOW *window);
void snake_free(snake_t *snake);

void snake_push_back(snake_t *snake, struct point body_coords);
void snake_push_front(snake_t *snake, struct point coords);
void snake_pop_back(snake_t *snake);

void snake_move(snake_t *snake, WINDOW *window);
void snake_grow(snake_t *snake, uint8_t new_elements);
void snake_print(snake_t *snake, WINDOW *window);

#endif
