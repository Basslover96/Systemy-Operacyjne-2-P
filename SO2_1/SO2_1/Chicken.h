#pragma once
#include <curses.h>
#include <cstdlib>
#include <ctime>

class Chicken
{
private:
	int leftX, topY;
	WINDOW * win;
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void moveLeftUp();
	void moveLeftDown();
	void moveRightUp();
	void moveRightDown();
	void drawChicken();
	void delay();
public:
	Chicken(WINDOW * window, int leftX, int topY);
	~Chicken();
	void live();
};

