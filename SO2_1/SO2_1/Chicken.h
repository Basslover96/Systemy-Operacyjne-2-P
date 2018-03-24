#pragma once
#include <curses.h>

class Chicken
{
private:
	int leftX, topY;
	WINDOW * win;
public:
	Chicken(WINDOW * window, int leftX, int topY);
	~Chicken();
	void chickenTest();
};

