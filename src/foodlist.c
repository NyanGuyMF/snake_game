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

	fl->count = 0;
	fl->max = max;
	fl->list = calloc(fl->max, sizeof(*fl->list));

	return fl;
}

void fl_free(foodlist_t *fl)
{
	for (uint8_t c = 0; c < fl->count;)
		free(fl->list[c++]);

	free(fl->list);
	free(fl);
}

bool fl_add(foodlist_t *fl, struct point coords)
{
	if (fl->count == fl->max)
		return false;

	food_t *food = food_new(&coords);

	fl->list[fl->count++] = food;
	return true;
}

void fl_rem(foodlist_t *fl, uint8_t index)
{
	if ((fl->count-1) < index)
		return;

	free(fl->list[index]);
	fl->list[index] = (void*)0x0;

	food_t **list = calloc(fl->max, sizeof(*fl->list));
	memcpy(list, fl->list, index * sizeof(*list));
	memcpy(list+index, fl->list+index+1, (fl->count-1 - index) * sizeof(*list));

	free(fl->list);

	fl->list = list;
	--fl->count;
}

void fl_clear(foodlist_t *fl, WINDOW *win)
{
	while (fl->count) {
		food_t *food = fl->list[--fl->count];
		wmove(win, food->y, food->x);
		waddch(win, ' ');
		free(food);
	}
}

void fl_print(foodlist_t *fl, WINDOW *w)
{
	for (uint8_t c = 0; c < fl->count; c++) {
		wmove(w, fl->list[c]->y, fl->list[c]->x);
		waddch(w, '%');
	}
}
