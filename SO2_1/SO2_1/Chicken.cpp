#include "Chicken.h"

Chicken::Chicken(WINDOW * window)
{
	win = window;
}

Chicken::~Chicken()
{
}

void Chicken::chickenTest()
{
	mvwprintw(win, 2, 6, "__//");
	mvwprintw(win, 3, 5, "/.__.\\");
	mvwprintw(win, 4, 5, "\\ \\/ /");
	mvwprintw(win, 5, 2, "'__/    \\");
	mvwprintw(win, 6, 3, "\\-      )");
	mvwprintw(win, 7, 4, "\\_____/");
	mvwprintw(win, 8, 6, "|  |");
	mvwprintw(win, 9, 6, "\"  \"");
}
