#include <curses.h>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Chicken.h"
#include <algorithm>

using namespace std;

int win_height, win_width;
int henhouse_height, henhouse_width;
int feeder_height, feeder_width;
WINDOW * henhouse;
WINDOW * feeder;
vector<Chicken> chickens;
const int number_of_chickens = 2;

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
		chickens[chickens.size() - 1].id = i;
		chickens[chickens.size()-1].direction = rand() % 2;
		chickens[chickens.size()-1].event_type = 0;
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

short checkEvent(Chicken &chicken)
{
	if(chicken.direction==0)
	{
		if ((chicken.leftX == 1) && (chicken.topY == 1)) return 5;
		if ((chicken.leftX == 1) && (chicken.topY == (henhouse_height - 13))) return 7;
		if (chicken.leftX == 1) return 1;
	}
	else
	{
		if ((chicken.leftX == (henhouse_width - 11)) && (chicken.topY == 1)) return 6;
		if ((chicken.leftX == (henhouse_width - 11)) && (chicken.topY == (henhouse_height - 13))) return 8;
		if (chicken.leftX == (henhouse_width - 11)) return 2;
	}
	if (chicken.topY == 1) return 3;
	if (chicken.topY == (henhouse_height - 13)) return 4;
	return 0;
}

void removeChicken(std::vector<Chicken> & chickens, int id)
{
	chickens.erase(remove_if(chickens.begin(), chickens.end(), [&](Chicken const & chicken) {return chicken.id == id; }), chickens.end());
}

void moveChicken(Chicken &chicken)
{
	while (chicken.eat > 0) {
		chicken.event_type = checkEvent(chicken);
		switch (chicken.event_type)
		{
			//Normalny ruch
		case 0:
			if (chicken.direction == 0)
			{
				chicken.leftX -= rand() % 2;
				chicken.topY += rand() % 3 - 1;
			}
			else
			{
				chicken.leftX += rand() % 2;
				chicken.topY += rand() % 3 - 1;
			}
			break;
			//Lewa krawêdŸ
		case 1:
			chicken.direction = 1;
			chicken.leftX += rand() % 2;
			chicken.topY += rand() % 3 - 1;
			break;
			//Prawa krawêdŸ
		case 2:
			chicken.direction = 0;
			chicken.leftX -= rand() % 2;
			chicken.topY += rand() % 3 - 1;
			break;
			//Górna krawêdŸ
		case 3:
			chicken.direction == 0 ? chicken.leftX -= rand() % 2 : chicken.leftX += rand() % 2;
			chicken.topY += rand() % 2;
			break;
			//Dolna krawêdŸ
		case 4:
			chicken.direction == 0 ? chicken.leftX -= rand() % 2 : chicken.leftX += rand() % 2;
			chicken.topY -= rand() % 2;
			break;
			//Lewy górny róg
		case 5:
			chicken.direction = 1;
			chicken.leftX += rand() % 2;
			chicken.topY += rand() % 2;
			break;
			//Prawy górny róg
		case 6:
			chicken.direction = 0;
			chicken.leftX -= rand() % 2;
			chicken.topY += rand() % 2;
			break;
			//Lewy dolny róg
		case 7:
			chicken.direction = 1;
			chicken.leftX += rand() % 2;
			chicken.topY -= rand() % 2;
			break;
			//Prawy dolny róg
		case 8:
			chicken.direction = 0;
			chicken.leftX -= rand() % 2;
			chicken.topY -= rand() % 2;
			break;
		}
		this_thread::sleep_for(std::chrono::microseconds(10));
	}
	removeChicken(chickens, chicken.id);
}



void draw()
{
	this_thread::sleep_for(std::chrono::milliseconds(10));
	while (chickens.size() > 0) {
		clear();
		refresh();
		drawHenhouse();
		drawFeeder();
		for (auto chicken : chickens)
		{
			if (chicken.direction == 0)
				drawLeftChicken(chicken);
			else
				drawRightChicken(chicken);
		}
		wrefresh(henhouse);
		wrefresh(feeder);
		this_thread::sleep_for(std::chrono::milliseconds(10));
	}
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
	
	thread draw_thread(draw);
	vector<thread> chickens_threads;
	for (auto chicken: chickens)
	{
		this_thread::sleep_for(std::chrono::milliseconds(500));
		chickens_threads.push_back(thread(moveChicken, &chicken));
	}

	getch();

	for(std:: thread & chicken_thread : chickens_threads)
	{
		if (chicken_thread.joinable())
			chicken_thread.join();
	}

	draw_thread.join();

	clear();
	endwin();
}