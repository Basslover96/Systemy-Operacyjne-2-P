#include "Chicken.h"

Chicken::Chicken(WINDOW * window, int leftX, int topY)
{
	win = window;
	this->leftX = leftX;
	this->topY = topY;
}

Chicken::~Chicken()
{
}

void Chicken::chickenTest()
{
	mvwprintw(win, topY, leftX + 4, "__//");
	mvwprintw(win, topY + 1, leftX + 3, "/.__.\\");
	mvwprintw(win, topY + 2, leftX + 3, "\\ \\/ /");
	mvwprintw(win, topY + 3, leftX, "'__/    \\");
	mvwprintw(win, topY + 4, leftX + 1, "\\-      )");
	mvwprintw(win, topY + 5, leftX + 2, "\\_____/");
	mvwprintw(win, topY + 6, leftX + 4, "|  |");
	mvwprintw(win, topY + 7, leftX + 4, "\"  \"");
}
