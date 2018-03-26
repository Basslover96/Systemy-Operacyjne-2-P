#include <curses.h>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Chicken.h"
#include "Player.h"
#include <string>

using namespace std;

int win_height, win_width;
int henhouse_height, henhouse_width;
int feeder_height, feeder_width;
WINDOW * henhouse;
WINDOW * feeder;
WINDOW * outside;
vector<Chicken> chickens;
Player player;
int number_of_chickens = 1;
int moves = 10;
int chickens_on_screen = 0;

//Menu pocz¹tkowe.
void setInitParameters()
{
	bool not_set = true;
	while (not_set) {
		clear();
		mvprintw(0, 0, "Klawiszami strzalka w gore i strzalka w dol ustaw liczbe kurczakow. Nastepnie zatwierdz strzalka w prawo.");
		string s = to_string(number_of_chickens);
		char const *pchar = s.c_str();
		mvprintw(1, 0, pchar);
		int input = getch();
		switch (input)
		{
		case KEY_UP:
			number_of_chickens++;
			break;
		case KEY_DOWN:
			if (number_of_chickens > 1)
				number_of_chickens--;
			break;
		case KEY_RIGHT:
			not_set = false;
			break;
		default:
			break;
		}
		refresh();
	}
	not_set = true;
	while (not_set) {
		clear();
		mvprintw(0, 0, "Klawiszami strzalka w gore i strzalka w dol ustaw liczbe ruchow. Nastepnie zatwierdz strzalka w prawo.");
		string s = to_string(moves);
		char const *pchar = s.c_str();
		mvprintw(1, 0, pchar);
		int input = getch();
		switch (input)
		{
		case KEY_UP:
			moves++;
			break;
		case KEY_DOWN:
			if (moves > 1)
				moves--;
			break;
		case KEY_RIGHT:
			not_set = false;
			break;
		default:
			break;
		}
		refresh();
	}
}

//Funkcje dotycz¹ce gracza.
void generatePlayer()
{
	player.currentPosition = rand() % 5 + 1;
	player.leftX = rand() % 113;
	player.topY = 1;
	player.direction = rand() % 2;
}
void movePlayer()
{
	while(chickens_on_screen>0)
	{
		int move = getch();
		switch(move)
		{
		case KEY_LEFT:
			player.direction = 0;
			if (player.leftX > 0)
				player.leftX -= 1;
			break;
		case KEY_RIGHT:
			player.direction = 1;
			if (player.leftX < 113)
				player.leftX += 1;
			break;
		case KEY_UP:
			if (player.topY > 0)
				player.topY -= 1;
			break;
		case KEY_DOWN:
			if (player.topY < 7)
				player.topY += 1;
			break;
		default:
			break;
		}
	}
}
void drawRightPlayer()
{
	mvwprintw(outside, player.topY, player.leftX+1, "(*_*)/");
	mvwprintw(outside, player.topY + 1, player.leftX + 1, "<) )");
	mvwprintw(outside, player.topY + 2, player.leftX + 1, "/  \\");
}
void drawLeftPlayer()
{
	mvwprintw(outside, player.topY, player.leftX, "\\(*_*)");
	mvwprintw(outside, player.topY + 1, player.leftX + 1, "(  (>");
	mvwprintw(outside, player.topY + 2, player.leftX + 1, "/  \\");
}

//Rysowanie okien.
void drawOutside()
{
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	wbkgd(outside, COLOR_PAIR(4));
}
void drawHenhouse()
{
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	wattron(henhouse, COLOR_PAIR(2));
	box(henhouse, 0, 0);
	wbkgd(henhouse, COLOR_PAIR(2));
	wattroff(henhouse, COLOR_PAIR(2));
}
void drawFeeder()
{
	getmaxyx(feeder, feeder_height, feeder_width);
	init_pair(3, COLOR_WHITE, COLOR_YELLOW);
	box(feeder, 0, 0);
	wbkgd(feeder, COLOR_PAIR(3));
	for (int i = 1; i < 5; i++)
	{
		wmove(feeder, 1, i * 24);
		wvline(feeder, 0, 2);
	}
};

//Funkcje dotycz¹ce kurczaków.
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
		chickens[chickens.size() - 1].moves = moves + 1;
		chickens[chickens.size() - 1].isVisible = true;
	}
	chickens_on_screen = number_of_chickens;
}
void drawLeftChicken(Chicken chicken)
{
	mvwprintw(henhouse, chicken.topY, chicken.leftX + 2, "\\\\__");
	mvwprintw(henhouse, chicken.topY + 1, chicken.leftX + 1, "/.__.\\");
	mvwprintw(henhouse, chicken.topY + 2, chicken.leftX + 1, "\\ \\/ /");
	mvwprintw(henhouse, chicken.topY + 3, chicken.leftX + 1, "/    \\_'");
	mvwprintw(henhouse, chicken.topY + 4, chicken.leftX, "(     -/");
	mvwprintw(henhouse, chicken.topY + 5, chicken.leftX + 1, "\\____/");
	mvwprintw(henhouse, chicken.topY + 6, chicken.leftX + 2, "|  |");
	mvwprintw(henhouse, chicken.topY + 7, chicken.leftX + 2, "\"  \"");
}
void drawRightChicken(Chicken chicken)
{
	mvwprintw(henhouse, chicken.topY, chicken.leftX + 4, "__//");
	mvwprintw(henhouse, chicken.topY + 1, chicken.leftX + 3, "/.__.\\");
	mvwprintw(henhouse, chicken.topY + 2, chicken.leftX + 3, "\\ \\/ /");
	mvwprintw(henhouse, chicken.topY + 3, chicken.leftX, "'__/    \\");
	mvwprintw(henhouse, chicken.topY + 4, chicken.leftX + 1, "\\-      )");
	mvwprintw(henhouse, chicken.topY + 5, chicken.leftX + 2, "\\_____/");
	mvwprintw(henhouse, chicken.topY + 6, chicken.leftX + 4, "|  |");
	mvwprintw(henhouse, chicken.topY + 7, chicken.leftX + 4, "\"  \"");
}
short checkEvent(Chicken chicken)
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
void moveChicken(Chicken& chicken)
{
	while (chicken.moves > 0) {
	
		chicken.moves--;
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
		if (chicken.moves == 0) {
			chicken.isVisible = false;
			chickens_on_screen--;
		}
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}

}

//Zarz¹dzanie oknami.
void clearAll()
{
	wclear(henhouse);
	wclear(feeder);
	wclear(outside);
}
void drawAll()
{
	drawHenhouse();
	drawFeeder();
	drawOutside();
}
void refreshAll()
{
	wrefresh(henhouse);
	wrefresh(feeder);
	wrefresh(outside);
}

//G³ówna funkcja rysuj¹ca.
void draw()
{
	this_thread::sleep_for(std::chrono::milliseconds(10));
	while (chickens_on_screen>0) {
		clearAll();
		drawAll();
		for (auto &chicken : chickens)
		{
			if(!chicken.isVisible)
				continue;
			(chicken.direction == 0) ? drawLeftChicken(chicken) : drawRightChicken(chicken);
		}
		(player.direction == 0) ? drawLeftPlayer() : drawRightPlayer();
		refreshAll();
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	clearAll();
	drawAll();
	refreshAll();
	this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main(int argc, char ** argv)
{	
	srand(time(nullptr));
	
	initscr();
	curs_set(0);
	keypad(stdscr, true);
	start_color();
	noecho();
	
	//Wywo³anie menu startowego - wybór liczby kurczaków i liczby ruchów.
	setInitParameters();

	//Stworzenie okien.
	getmaxyx(stdscr, win_height, win_width);
	henhouse = newwin(win_height - 10, win_width, 0, 0);
	feeder = newwin(4, win_width - 2, win_height - 15, 1);
	outside = newwin(10, win_width, win_height - 10, 0);

	//Generowanie kurczaków i gracza.
	generatePlayer();
	generateChickens();

	//Tworzenie w¹tków.
	thread draw_thread(draw);
	thread player(movePlayer);
	vector<thread> chickens_threads;
	for(Chicken & chicken : chickens)
	{
		this_thread::sleep_for(std::chrono::milliseconds(500));
		chickens_threads.emplace_back(thread(moveChicken, std::ref(chicken)));
	}

	//Koñczenie pracy w¹tków.
	for(std:: thread & chicken_thread : chickens_threads)
	{
		if (chicken_thread.joinable())
			chicken_thread.join();
	}
	player.join();
	draw_thread.join();

	keypad(stdscr, false);
	clear();
	endwin();
}