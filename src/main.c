#include <ncurses.h>
#include "snake.h"
#include "game.h"

#define HEADER_LINES 4

static WINDOW *header;
static WINDOW *game_screen;

int main(void)
{
	initscr();
	curs_set(0);
	srand(time((void*)0x0));

	header = newwin(HEADER_LINES, COLS, 0, 0);
	game_screen = newwin(LINES - HEADER_LINES - 1, COLS - 1, HEADER_LINES + 1, 0);

	noecho();
	cbreak();
	refresh();

	game_t *game = game_new(header, game_screen);
	game_start(game);
	game_end(game);

	delwin(header);
	delwin(game_screen);
	endwin();

	return 0;
}
