#include <curses.h>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Chicken.h"

using namespace std;

int win_height, win_width;
int henhouse_height, henhouse_width;
int feeder_height, feeder_width;
WINDOW * henhouse;
WINDOW * feeder;
vector<Chicken> chickens;
const int number_of_chickens = 10;

void drawHenhouse()
{
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	box(henhouse, 0, 0);
	wbkgd(henhouse, COLOR_PAIR(2));
}
void generateChickens()
{
	getmaxyx(henhouse, henhouse_height, henhouse_width);
	for (int i = 0; i < number_of_chickens; i++) {
		chickens.emplace_back(Chicken());
		chickens[chickens.size()-1].direction = rand() % 2;
		chickens[chickens.size()-1].move_type = 0;
		chickens[chickens.size()-1].topY = rand() % (henhouse_height - 13) + 1;
		chickens[chickens.size()-1].leftX = rand() % (henhouse_width - 11) + 1;
		chickens[chickens.size() - 1].colorR = rand() % 998 + 1;
		chickens[chickens.size() - 1].colorG = rand() % 998 + 1;
		chickens[chickens.size() - 1].colorB = rand() % 998 + 1;
	}
}
void drawLeftChicken(Chicken chicken)
{
	init_color(COLOR_RED, chicken.colorR, chicken.colorB, chicken.colorB);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	mvwprintw(henhouse, chicken.topY, chicken.leftX + 2, "\\\\__");
	mvwprintw(henhouse, chicken.topY + 1, chicken.leftX + 1, "/.__.\\");
	mvwprintw(henhouse, chicken.topY + 2, chicken.leftX + 1, "\\ \\/ /");
	mvwprintw(henhouse, chicken.topY + 3, chicken.leftX + 1, "/    \\_'");
	mvwprintw(henhouse, chicken.topY + 4, chicken.leftX, "(     -/");
	mvwprintw(henhouse, chicken.topY + 5, chicken.leftX + 1, "\\____/");
	mvwprintw(henhouse, chicken.topY + 6, chicken.leftX + 2, "|  |");
	mvwprintw(henhouse, chicken.topY + 7, chicken.leftX + 2, "\"  \"");
	attroff(COLOR_PAIR(1));
}
void drawRightChicken(Chicken chicken)
{
	init_color(COLOR_RED, chicken.colorR, chicken.colorB, chicken.colorB);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	mvwprintw(henhouse, chicken.topY, chicken.leftX + 4, "__//");
	mvwprintw(henhouse, chicken.topY + 1, chicken.leftX + 3, "/.__.\\");
	mvwprintw(henhouse, chicken.topY + 2, chicken.leftX + 3, "\\ \\/ /");
	mvwprintw(henhouse, chicken.topY + 3, chicken.leftX, "'__/    \\");
	mvwprintw(henhouse, chicken.topY + 4, chicken.leftX + 1, "\\-      )");
	mvwprintw(henhouse, chicken.topY + 5, chicken.leftX + 2, "\\_____/");
	mvwprintw(henhouse, chicken.topY + 6, chicken.leftX + 4, "|  |");
	mvwprintw(henhouse, chicken.topY + 7, chicken.leftX + 4, "\"  \"");
	attroff(COLOR_PAIR(1));
}
void drawFeeder()
{
	getmaxyx(feeder, feeder_height, feeder_width);
	init_pair(3, COLOR_WHITE, COLOR_YELLOW);
	box(feeder, 0, 0);
	wbkgd(feeder,COLOR_PAIR(3));
	for (int i = 1; i < 5; i++)
	{
		wmove(feeder, 1, i * 24);
		wvline(feeder,0, 2);
	}
};

void updateChickenParams()
{
	
}

void draw()
{
	clear();
	refresh();
	drawHenhouse();
	drawFeeder();
	for (auto chicken: chickens)
	{
		if (chicken.direction == 0)
			drawLeftChicken(chicken);
		else
			drawRightChicken(chicken);
	}
	wrefresh(henhouse);
	wrefresh(feeder);
}

int main(int argc, char ** argv)
{	
	srand(time(nullptr));
	initscr();
	cbreak();
	start_color();
	noecho();
	getmaxyx(stdscr, win_height, win_width);
	henhouse = newwin(win_height - 10, win_width, 0, 0);
	feeder = newwin(4, win_width-2, win_height - 15, 1);
	generateChickens();
	draw();


	getch();
	endwin();
}