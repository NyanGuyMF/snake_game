#include "game.h"

static struct timespec sleep_for;

static char inpause[] = "Pause. Press P to unpase. ";
static char on_unpause[] = "Continue in..   ";
/** sizeof returns string size with \0, but we need strlen for compile time. */
static size_t inpause_len = sizeof(inpause) - 1;
static size_t on_unpause_len = sizeof(on_unpause) - 1;

static char *uint_to_str(uint32_t num)
{
	char *str = calloc(11, sizeof(char));

	int8_t c = 9;
	do {
		str[c--] = (num % 10) + '0';
		num /= 10;
	} while (num > 0);
	while (c > -1) {
		str[c--] = ' ';
	}

	return str;
}

static void pause(WINDOW *header)
{
	// show user status message
	size_t offset = header->_maxx - inpause_len;
	wmove(header, 1, offset);
	waddstr(header, inpause);
	wrefresh(header);
	// wait user to press P
	nodelay(stdscr, false);
	getch();

	// clear pause status message
	wmove(header, 1, offset);
	/* -1 because we don't need to remove | char */
	for (size_t cursor = 0; cursor < inpause_len - 1; cursor++)
		waddch(header, ' ');
	wrefresh(header);

	offset = header->_maxx - on_unpause_len;
	wmove(header, 1, offset);
	waddstr(header, on_unpause);
	wrefresh(header);
	static char sec_offset[] = "|";
	size_t sec_offset_len = header->_maxx - sizeof(sec_offset);
	sleep_for.tv_nsec = 0;
	sleep_for.tv_sec = _PAUSE_COUNTER_DELAY;
	for (char c = '3'; c > '0'; c--) {
		wmove(header, 1, sec_offset_len);
		waddch(header, c);
		wrefresh(header);
		nanosleep(&sleep_for, NULL);
	}

	// clear pause status message
	wmove(header, 1, offset);
	/* -1 because we don't need to remove | char */
	for (size_t cursor = 0; cursor < on_unpause_len - 1; cursor++)
		waddch(header, ' ');
	wrefresh(header);

	sleep_for.tv_sec = 0;
}

static void process_user_input(game_t *game)
{
	nodelay(stdscr, true);

	switch (getch()) {
	case 'W':
	case 'w':
	case KEY_UP:
		if (game->snake->direction != DOWN)
			game->snake->direction = UP;
		break;
	case 'S':
	case 's':
	case KEY_DOWN:
		if (game->snake->direction != UP)
			game->snake->direction = DOWN;
		break;
	case 'A':
	case 'a':
	case KEY_LEFT:
		if (game->snake->direction != RIGHT)
			game->snake->direction = LEFT;
		break;
	case 'D':
	case 'd':
	case KEY_RIGHT:
		if (game->snake->direction != LEFT)
			game->snake->direction = RIGHT;
		break;
	case 'P':
	case 'p':
		pause(game->header);
		break;
	}
}

static bool is_snakehead_collide(snake_t *snake)
{
	snake_body_t *cur = snake->head->next;

	while (cur) {
		if (point_equals(&cur->coords, &snake->head->coords))
			return true;
		cur = cur->next;
	}

	return false;
}

static void eat_food(game_t *game, uint8_t food_index)
{
	food_t *food = game->food->food[food_index];
	wmove(game->game_screen, food->y, food->x);
	waddch(game->game_screen, ' ');
	fl_rem(game->food, food_index);
	snake_grow(game->snake, _SNAKE_GROW_LEN);
}

static void collision_detection(game_t *game)
{
	if (is_snakehead_collide(game->snake)) {
		game->snake->is_alive = false;
		++game->death;
		return;
	}

	struct point *head_coord = &game->snake->head->coords;
	food_t **list = game->food->food;
	for (uint8_t c = 0; c < game->food->size; c++) {
		if (point_equals(list[c], head_coord)) {
			eat_food(game, c);
			++game->score;
			break;
		}
	}
}

static bool is_point_collide(game_t *game, struct point *point)
{
	snake_body_t *cur = game->snake->head;
	while (cur) {
		if (point_equals(&cur->coords, point))
			return true;
		cur = cur->next;
	}

	for (uint8_t c = 0; c < game->food->size; c++)
		if (point_equals(game->food->food[c], point))
			return true;

	return false;
}

static void generate_food(game_t *game)
{
	if (game->food->size >= _FOOD_MAX)
		return;

	uint8_t chance = rand() % 100;

	if (game->food->size > 3) {
		if (chance >= 1)
			return;
	} else if (game->food->size > 0) {
		if (chance >= 5)
			return;
	} else {
		if (chance >= 25)
			return;
	}

	struct point point;
	do {
		point.x = rand() % game->game_screen->_maxx;
		point.y = rand() % game->game_screen->_maxy;
	} while (is_point_collide(game, &point));

	fl_add(game->food, point);
}

static void delay(direction_t *direct)
{
	if ((*direct == UP) || (*direct == DOWN))
		sleep_for.tv_nsec = _DELAY_VERT;
	else
		sleep_for.tv_nsec = _DELAY_HORIZON;

	nanosleep(&sleep_for, NULL);
}

static void update_score(game_t *game)
{
	char *buf = uint_to_str(game->score);

	wmove(game->header, 1, sizeof("| SCORE:"));
	waddstr(game->header, buf);
	wrefresh(game->header);
	free(buf);

	buf = uint_to_str(game->death);
	wmove(game->header, 2, sizeof("| DEATH:"));
	waddstr(game->header, buf);
	wrefresh(game->header);
	free(buf);
}

static void update_screen(game_t *game)
{
	snake_print(game->snake, game->game_screen);
	fl_print(game->food, game->game_screen);
	update_score(game);
}

static void game_process(game_t *game)
{
	delay(&game->snake->direction);

	generate_food(game);
	process_user_input(game);
	snake_move(game->snake, game->game_screen);
	collision_detection(game);
	update_screen(game);

	wrefresh(game->game_screen);
}

game_t *game_new(WINDOW *h, WINDOW *g)
{
	game_t *game = malloc(sizeof(game_t));

	game->snake = snake_new_centered(_SNAKE_DEFAULT_LEN, g);
	game->food = fl_new(_FOOD_MAX);
	game->game_screen = g;
	game->header = h;

	return game;
}

static void init_score(game_t *game)
{
	wmove(game->header, 0, 0);
	waddch(game->header, '+');
	for (uint8_t c = 0; c < (game->header->_maxx-1); c++)
		waddch(game->header, '-');
	waddch(game->header, '+');

	waddch(game->header, '|');
	waddstr(game->header, " SCORE:");
	wmove(game->header, 1, game->header->_maxx);
	waddch(game->header, '|');
	wmove(game->header, 2, 0);
	waddch(game->header, '|');
	waddstr(game->header, " DEATH:");
	wmove(game->header, 2, game->header->_maxx);
	waddch(game->header, '|');

	wmove(game->header, 3, 0);
	waddch(game->header, '+');
	for (uint8_t c = 0; c < (game->header->_maxx-1); c++)
		waddch(game->header, '-');
	waddch(game->header, '+');

	wrefresh(game->header);
}

static bool is_restart(game_t *game)
{
	char press_space[23] = "Press SPACE to restart";
	WINDOW *win = game->game_screen;
	wmove(win, win->_maxy >> 1, (win->_maxx >> 1) - (sizeof(press_space) >> 1));
	waddstr(win, press_space);
	char press_esc[18] = "Press ESC to exit";
	wmove(win, (win->_maxy >> 1) + 1, (win->_maxx >> 1) - (sizeof(press_esc) >> 1));
	waddstr(win, press_esc);
	nodelay(stdscr, false);
	wrefresh(win);

	int ch;
	do {
		ch = getch();
	} while ((ch != _ESC) && (ch != _SPACE));

	return ch != _ESC;
}

void game_start(game_t *game)
{
	restart:
	init_score(game);
	while (game->snake->is_alive)
		game_process(game);

	if (is_restart(game)) {
		werase(game->header);
		werase(game->game_screen);

		fl_clear(game->food, game->game_screen);
		snake_free(game->snake);
		game->snake = snake_new_centered(_SNAKE_DEFAULT_LEN, game->game_screen);
		game->score = 0;

		goto restart;
	}
}

void game_end(game_t *game)
{
	fl_free(game->food);
	snake_free(game->snake);
	free(game);
}
