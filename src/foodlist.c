#include "foodlist.h"

static food_t *food_new(struct point *point)
{
	food_t *food = malloc(sizeof(food_t));

	food->x = point->x;
	food->y = point->y;

	return food;
}

foodlist_t *fl_new(uint8_t max)
{
	foodlist_t *fl = malloc(sizeof(foodlist_t));

	fl->size = 0;
	fl->cap = max;
	fl->food = calloc(fl->cap, sizeof(*fl->food));

	return fl;
}

void fl_free(foodlist_t *fl)
{
	for (uint8_t c = 0; c < fl->size; c++)
		free(fl->food[c]);

	free(fl->food);
	free(fl);
}

bool fl_add(foodlist_t *fl, struct point coords)
{
	if (fl->size == fl->cap)
		return false;

	food_t *food = food_new(&coords);

	fl->food[fl->size++] = food;
	return true;
}

void fl_rem(foodlist_t *fl, uint8_t index)
{
	if (index >= fl->size)
		return;

	free(fl->food[index]);

	if (fl->size > 1)
		fl->food[index] = fl->food[fl->size-1];
	--fl->size;
}

void fl_clear(foodlist_t *fl, WINDOW *win)
{
	while (fl->size--) {
		food_t *food = fl->food[fl->size];
		wmove(win, food->y, food->x);
		waddch(win, ' ');
		free(food);
	}
}

void fl_print(foodlist_t *fl, WINDOW *w)
{
	for (uint8_t c = 0; c < fl->size; c++) {
		wmove(w, fl->food[c]->y, fl->food[c]->x);
		waddch(w, '%');
	}
}
