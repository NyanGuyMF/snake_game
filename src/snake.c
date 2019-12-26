#include "snake.h"

snake_t *snake_new(uint16_t len, uint16_t head_x, uint16_t head_y)
{
	snake_t *snake = malloc(sizeof(snake_t));
	snake->head = NULL;
	snake->tail = NULL;
	snake->is_alive = 1;
	snake->direction = LEFT;
	struct point head_pos = {.x=head_x, .y=head_y};

	for (uint16_t c = 0; c < len; c++) {
		snake_push_back(snake, head_pos);
		++head_pos.x;
	}

	return snake;
}

snake_t *snake_new_centered(uint16_t len, WINDOW *w)
{
	uint16_t head_x = w->_maxx >> 1;
	uint16_t head_y = (w->_maxy >> 1);

	return snake_new(len, head_x, head_y);
}

void snake_free(snake_t *snake)
{
	snake_body_t *current = snake->head;
	snake_body_t *next = current;

	while ((current = next)) {
		next = current->next;
		free(current);
	}

	free(snake);
}

void snake_push_back(snake_t *snake, struct point coords)
{
	snake_body_t *new_tail = malloc(sizeof(snake_body_t));
	new_tail->coords = coords;
	new_tail->next = NULL;

	if (!snake->head) {
		snake->head = new_tail;
	} else if (!snake->tail) {
		snake->tail = new_tail;
		snake->head->next = snake->tail;
	} else {
		snake->tail->next = new_tail;
		snake->tail = new_tail;
	}

	++snake->len;
}

void snake_push_front(snake_t *s, struct point p)
{
	snake_body_t *new_head = malloc(sizeof(snake_body_t));
	new_head->coords = p;
	new_head->next = s->head;
	s->head = new_head;
	++s->len;
}

void snake_pop_back(snake_t *s)
{
	snake_body_t *before_tail = s->head;

	while (before_tail->next != s->tail)
		before_tail = before_tail->next;

	free(before_tail->next);
	before_tail->next = NULL;
	s->tail = before_tail;
}

void snake_change_direction(snake_t *snake, direction_t new_direction)
{
	/* LEFT <-> RIGHT, or UP <-> DOWN */
	if (abs((int) new_direction - (int) snake->direction) != 2)
		snake->direction = new_direction;
}

void snake_move(snake_t *s, WINDOW *w)
{
	// remove previous tail
	wmove(w, s->tail->coords.y, s->tail->coords.x);
	waddch(w, ' ');

	struct point new_head = s->head->coords;

	if (s->direction == LEFT) {
		if (new_head.x != 0)
			--new_head.x;
		else
			new_head.x = w->_maxx;
	} else if (s->direction == RIGHT) {
		if (new_head.x != w->_maxx)
			++new_head.x;
		else
			new_head.x = 0;
	} else if (s->direction == UP) {
		if (new_head.y != 0)
			--new_head.y;
		else
			new_head.y = w->_maxy;
	} else {
		if (new_head.y != w->_maxy)
			++new_head.y;
		else
			new_head.y = 0;
	}

	snake_push_front(s, new_head);
	snake_pop_back(s);
}

void snake_grow(snake_t *s, uint8_t new_elements)
{
	for (uint8_t c = 0; c < new_elements; c++)
		snake_push_back(s, s->tail->coords);
}

void snake_print(snake_t *s, WINDOW *w)
{
	char body, head;
	if (s->is_alive) {
		body = 'O';
		head = '@';
	} else {
		body = '+';
		head = 'X';
	}

	snake_body_t *cur = s->head;
	cur = cur->next;

	while (cur) {
		wmove(w, cur->coords.y, cur->coords.x);
		waddch(w, body);
		cur = cur->next;
	}

	cur = s->head;
	wmove(w, cur->coords.y, cur->coords.x);
	waddch(w, head);
}
