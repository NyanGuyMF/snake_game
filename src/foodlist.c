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

static void fl_shift_left(foodlist_t *food_list, uint8_t dest_pos, uint8_t food_to_shift)
{
	food_t **dest = food_list->list+dest_pos;
	food_t **src = dest+food_to_shift;
	size_t nfood = (food_list->count - (dest_pos + food_to_shift));

	memmove(dest, src, nfood * sizeof(*food_list->list));
}

void fl_rem(foodlist_t *fl, uint8_t index)
{
	if (index >= fl->count)
		return;

	free(fl->list[index]);

	// 1 means we removed one element,
	// so we need to shift only by one element
	fl_shift_left(fl, index, 1);

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
