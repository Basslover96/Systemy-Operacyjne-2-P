#include <curses.h>
#include <thread>
#include <vector>
#include <ctime>
#include "Chicken.h"
#include "Player.h"
#include <string>
#include <mutex>

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
vector<short> eat = { 0,0,0,0 };
mutex m;
mutex feeder_mutex;
condition_variable cv_feeder;
bool load_feed = false;
bool f_pressed;
int move_sign;
bool end_game = false;


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
}

//Nape³nienie koryta pasz¹.
void loadFeed(int troughNumber)
{
	if (eat[troughNumber] == 0) {
		eat[troughNumber] += 1;
		this_thread::sleep_for(std::chrono::milliseconds(500));
		eat[troughNumber] += 1;
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//G³ówna funkcja karmienia.
void loadFeeder()
{
	while (!end_game) {
		unique_lock<mutex> lk(feeder_mutex);
		while (!load_feed && !end_game)
		{
			cv_feeder.wait(lk);
		};
		load_feed = false;
		if (player.topY == 0) {
			if (player.leftX + 2 < 1 * 29)
			{
				loadFeed(0);
			}
			else
				if ((player.leftX + 2 > 1 * 29) && (player.leftX + 2 < 2 * 29)) {
					loadFeed(1);
				}
				else
					if ((player.leftX + 2 > 2 * 29) && (player.leftX + 2 < 3 * 29))
					{
						loadFeed(2);
					}
					else
						if (player.leftX + 2 > 3 * 29)
						{
							loadFeed(3);
						}
		}
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
	while (chickens_on_screen > 0 && !end_game)
	{
		move_sign = getch();
		switch (move_sign)
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
		case 'f':
			if (((player.leftX + 2) != 1 * 29) && ((player.leftX + 2 != 2 * 29)) && ((player.leftX + 2) != 3 * 29)) {
				f_pressed = true;
				move_sign = 32;
			}
			break;
		case 'q': {
			end_game = true;
		}
			break;
		default:
			break;
		}
		if (f_pressed) {
			{
				unique_lock<mutex> lk(feeder_mutex);
				load_feed = true;
				f_pressed = false;
			}
			cv_feeder.notify_one();
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
	init_pair(120, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(120));
	wbkgd(outside, COLOR_PAIR(120));
	attroff(COLOR_PAIR(120));
}
void drawHenhouse()
{
	init_pair(121, COLOR_WHITE, COLOR_GREEN);
	wattron(henhouse, COLOR_PAIR(121));
	box(henhouse, 0, 0);
	wbkgd(henhouse, COLOR_PAIR(121));
	wattroff(henhouse, COLOR_PAIR(121));
}
void drawFeeder()
{
	getmaxyx(feeder, feeder_height, feeder_width);
	init_pair(122, COLOR_WHITE, COLOR_YELLOW);
	wattron(feeder, COLOR_PAIR(122));
	box(feeder, 0, 0);
	wbkgd(feeder, COLOR_PAIR(122));
	wattroff(feeder,COLOR_PAIR(122));
	for (int i = 1; i < 4; i++)
	{
		wmove(feeder, 1, i * 29);
		wvline(feeder, 0, 2);
	}
	for (int i = 0; i<4; i++)
	{
		if(eat[i]==1)
		{
			if (i < 3) {
				mvwprintw(feeder, 1, i * 29 + 1, "**************");
				mvwprintw(feeder, 2, i * 29 + 1, "**************");
			}
			else
			{
				mvwprintw(feeder, 1, i * 29 + 1, "**************");
				mvwprintw(feeder, 2, i * 29 + 1, "**************");
			}
		}
		else
			if(eat[i]==2)
			{
				if (i < 3) {
					mvwprintw(feeder, 1, i * 29 + 1, "****************************");
					mvwprintw(feeder, 2, i * 29 + 1, "****************************");
				}
				else
				{
					mvwprintw(feeder, 1, i * 29 + 1, "*****************************");
					mvwprintw(feeder, 2, i * 29 + 1, "*****************************");
				}
			}
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
		chickens[chickens.size() - 1].food = rand() % 50 + 70;
		chickens[chickens.size() - 1].isVisible = true;
		chickens[chickens.size() - 1].isHungry = false;
		chickens[chickens.size() - 1].color = 0;
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
void tryToEat(Chicken& chicken, int localizationCorrection)
{
	if (chicken.leftX + localizationCorrection < 29)
	{
		if (eat[0] == 2)
		{
			this_thread::sleep_for(std::chrono::milliseconds(500));
			eat[0] -= 1;
			chicken.food += 30;
			this_thread::sleep_for(std::chrono::milliseconds(500));
			eat[0] -= 1;
			chicken.food += 30;
			chicken.isHungry = false;
		}
	}
	else {
		if (chicken.leftX + localizationCorrection >= 29 && chicken.leftX + 4 < 58)
		{
			if (eat[1] == 2)
			{
				this_thread::sleep_for(std::chrono::milliseconds(500));
				eat[1] -= 1;
				chicken.food += 30;
				this_thread::sleep_for(std::chrono::milliseconds(500));
				eat[1] -= 1;
				chicken.food += 30;
				chicken.isHungry = false;
			}
		}
		else
			if (chicken.leftX + localizationCorrection >= 58 && chicken.leftX + 4 < 87)
			{
				if (eat[2] == 2)
				{
					this_thread::sleep_for(std::chrono::milliseconds(500));
					eat[2] -= 1;
					chicken.food += 30;
					this_thread::sleep_for(std::chrono::milliseconds(500));
					eat[2] -= 1;
					chicken.food += 30;
					chicken.isHungry = false;
				}
			}
			else
			{
				if (eat[3] == 2)
				{
					this_thread::sleep_for(std::chrono::milliseconds(500));
					eat[3] -= 1;
					chicken.food += 30;
					this_thread::sleep_for(std::chrono::milliseconds(500));
					eat[3] -= 1;
					chicken.food += 30;
					chicken.isHungry = false;
				}
			}
	}
}
void moveChicken(Chicken& chicken)
{
	while (chicken.food > 0 && !end_game) {
		if(chicken.food<40)
		{
			chicken.color = 1;
			chicken.isHungry = true;
			while (chicken.isHungry) {
				chicken.food--;
				if (chicken.food < 20 && chicken.color == 1)
				{
					chicken.color = 2;
				}
				if (chicken.topY < (henhouse_height - 13)) {
					chicken.topY += 1;
				}
				else
				{
					m.lock();
					if (chicken.direction == 0)
						tryToEat(chicken, 4);
					else
						tryToEat(chicken, 6);
					m.unlock();
				}
				if (chicken.food == 0) {
					chicken.isVisible = false;
					chickens_on_screen--;
				}
				this_thread::sleep_for(std::chrono::milliseconds(500));
			}
		}
		else
		{
			chicken.color = 0;
			chicken.food--;
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
			if (chicken.food == 0) {
				chicken.isVisible = false;
				chickens_on_screen--;
			}
			this_thread::sleep_for(std::chrono::milliseconds(500));
		}
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
	wrefresh(henhouse);
	drawFeeder();
	wrefresh(feeder);
	drawOutside();		
	wrefresh(outside);
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
	while (chickens_on_screen>0 && !end_game) {

		clearAll();
		drawAll();
		for (auto &chicken : chickens)
		{
			if(!chicken.isVisible)
				continue;
			//Wybór koloru kurczaka w zale¿noœci od poziomu najedzenia.
			switch (chicken.color)
			{
			case 1:
			{
				init_pair(123, COLOR_YELLOW, COLOR_GREEN);
				wattron(henhouse, COLOR_PAIR(123));
			}
			break;
			case 2:
			{
				init_pair(124, COLOR_RED, COLOR_GREEN);
				wattron(henhouse, COLOR_PAIR(124));
			}
			break;
			default:
			{
				init_pair(125, COLOR_WHITE, COLOR_GREEN);
				wattron(henhouse, COLOR_PAIR(125));
			}
			break;
			}
			//Rysowanie kurczaka.
			(chicken.direction == 0) ? drawLeftChicken(chicken) : drawRightChicken(chicken);
			wattroff(henhouse,COLOR_PAIR(chicken.id));
		}
		(player.direction == 0) ? drawLeftPlayer() : drawRightPlayer();
		refreshAll();
		this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	clearAll();
	drawAll();
	refreshAll();
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
	thread feeder(loadFeeder);
	vector<thread> chickens_threads;
	for (Chicken & chicken : chickens)
	{
		this_thread::sleep_for(std::chrono::milliseconds(500));
		chickens_threads.emplace_back(thread(moveChicken, std::ref(chicken)));
	}

	//Koñczenie pracy w¹tków.
	for (std::thread & chicken_thread : chickens_threads)
	{
		if (chicken_thread.joinable())
			chicken_thread.join();
	}
	//if (feeder.joinable()) {
	//feeder.join();
	//};
	player.join();
	draw_thread.join();

	keypad(stdscr, false);
	clear();
	endwin();
	exit(0);
}