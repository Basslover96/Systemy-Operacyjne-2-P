#include "Controller.h"


void Controller::initialization()
{
	initscr();
	cbreak();
	start_color();
	noecho();
	getmaxyx(stdscr, winHeight, winWidth);
	winStartX = winStartY = 0;
	win = newwin(winHeight - 10, winWidth, winStartY, winStartX);
	refresh();
}

void Controller::createBorder()
{
	box(win, 0, 0);
}

void Controller::createFeeder()
{
	move(winHeight - 15, 1);
	hline(0, 118);
	for (int i = 0; i < 5; i++)
	{
		move(winHeight - 14, i * 24);
		vline(0, 3);
	}
}

void Controller::chickenTest()
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

Controller::Controller()
{
	initialization();
	createBorder();
	createFeeder();
	chickenTest();
}


Controller::~Controller()
{
	endwin();
}
