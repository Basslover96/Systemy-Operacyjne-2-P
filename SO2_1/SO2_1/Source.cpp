#include <curses.h>
#include <thread>
#include "Controller.h"
#include "Chicken.h"

int main(int argc, char ** argv) {
	Controller * controller = new Controller();
	Chicken * chicken = new Chicken(controller->getWin(),4,4);
	std::thread t1(&Chicken::live,chicken);
	wrefresh(controller->getWin());
	while (getch() != 'q'){};
	t1.join();
	return 0;
}