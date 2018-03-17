#include "Controller.h"

void Controller::createBorder() const
{
	box(win, 0, 0);
}

void Controller::createFeeder() const
{
	move(winHeight - 15, 1);
	hline(0, 118);
	for (int i = 0; i < 5; i++)
	{
		move(winHeight - 14, i * 24);
		vline(0, 3);
	}
}

Controller::Controller()
{
	initscr();
	cbreak();
	start_color();
	noecho();
	getmaxyx(stdscr, winHeight, winWidth);
	winStartX = winStartY = 0;
	win = newwin(winHeight - 10, winWidth, winStartY, winStartX);
	refresh();
	createBorder();
	createFeeder();
}


Controller::~Controller()
{
	endwin();
}

WINDOW * Controller::getWin() const
{
	return win;
}
