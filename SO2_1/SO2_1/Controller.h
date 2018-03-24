#pragma once
#include <curses.h>

class Controller
{
private:
	int winHeight, winWidth, winStartX, winStartY;
	WINDOW * win;
	void createBorder() const;
	void createFeeder() const;
public:
	Controller();
	~Controller();
	WINDOW * getWin() const;
};

