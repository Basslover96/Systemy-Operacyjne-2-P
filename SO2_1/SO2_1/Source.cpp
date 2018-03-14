#include <curses.h>
#include <thread>
#include "Controller.h"

int main(int argc, char ** argv) {
	Controller controller;
	wrefresh(controller.win);
	getch();
	return 0;
}