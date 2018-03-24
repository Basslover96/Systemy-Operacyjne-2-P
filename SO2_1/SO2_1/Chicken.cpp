#include "Chicken.h"
#include <chrono>

Chicken::Chicken(WINDOW * window, int leftX, int topY)
{
	srand(time(NULL));
	win = window;
	this->leftX = leftX;
	this->topY = topY;
}

Chicken::~Chicken()
{
}

void Chicken::live()
{
	int moveId = rand() % 8;
	switch(moveId)
	{
	case 0:
		moveLeft();
		break;
	case 1:
		moveRight();
		break;
	case 2:
		moveUp();
		break;
	case 3:
		moveDown();
		break;
	case 4:
		moveLeftUp();
		break;
	case 5:
		moveLeftDown();
		break;
	case 6:
		moveRightUp();
		break;
	case 7:
		moveRightDown();
		break;
	}
	delay();
}

void Chicken::moveLeft()
{
	leftX = leftX - 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveRight()
{
	leftX = leftX + 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveUp()
{
	topY = topY - 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveDown()
{
	topY = topY + 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveLeftUp()
{
	leftX = leftX - 1;
	topY = topY - 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveLeftDown()
{
	leftX = leftX - 1;
	topY = topY + 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveRightUp()
{
	leftX = leftX + 1;
	topY = topY - 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::moveRightDown()
{
	leftX = leftX - 1;
	topY = topY + 1;
	drawChicken();
	wrefresh(win);
}

void Chicken::delay()
{
	for (long int i = 0; i < 200000000; i++);

}

void Chicken::drawChicken()
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
