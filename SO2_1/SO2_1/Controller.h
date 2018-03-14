#pragma once
#include <curses.h>

class Controller
{
private:
	int winHeight, winWidth, winStartX, winStartY;
	
	void initialization();
	void createBorder();
	void createFeeder();
	void chickenTest();
public:
	WINDOW * win;
	Controller();
	~Controller();
};

