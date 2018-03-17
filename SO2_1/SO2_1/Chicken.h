#pragma once
#include <curses.h>

class Chicken
{
private:
	WINDOW * win;
public:
	Chicken(WINDOW * window);
	~Chicken();
	void chickenTest();
};

