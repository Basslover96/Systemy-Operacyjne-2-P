#include <curses.h>
#include <thread>
#include "Controller.h"
#include "Chicken.h"

int main(int argc, char ** argv) {
	Controller * controller = new Controller();
	Chicken * chicken = new Chicken(controller->getWin(),4,4);
	chicken->chickenTest();
	wrefresh(controller->getWin());
	getch();
	return 0;
}