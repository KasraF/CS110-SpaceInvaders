//Team ByteSized - Sam Jean Kasra

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>
#include "windows.h"
using namespace std;

const int INIT_INVADERS = 5;

struct invObj
{

	const char alien = 'A';
	bool alive = true;
	int x = 0;
	int y = 0;
	bool moveright = true;
	void kill(vector<string> &tileMap)
	{
		tileMap[y][x] = ' ';
		alive = false;
	}

	void WriteToMap(vector<string> &tileMap)
	{
		if (alive)
			tileMap[y][x] = alien;
	}

	void Move(vector<string> &tileMap)
	{
		if (alive)
		{
			tileMap[y][x] = ' ';
		}
		x += ((moveright) ? 1 : -1);
	}

	bool CanMove(vector<string> &tileMap, bool &game_running)
	{

		bool ableToMove = true;

		switch (tileMap[y][x + ((moveright) ? 1 : -1)])
		{
		case '#':
			ableToMove = false;
			break;
		case '^':
			game_running = false;
			break;
		default:
			break;
		}

		return ableToMove;

	}



};




struct bulletObj
{
	int x;
	int y;
	bool enemyBullet = false;
	bool isActive = false;
	char icon = '!';
	void kill(vector<string> &tileMap)
	{
		tileMap[y][x] = ' ';
		isActive = false;
	}
	void Step(vector<string> &tileMap, vector<invObj> &invaders, int &score, bool &game_running)
	{
		int yDir = (enemyBullet) ? 1 : -1;

		bool found = false;
		switch (tileMap[y + yDir][x])
		{



		case 'A':
			found = false;
			if (!enemyBullet){
				for (int i = 0; i < invaders.size() && !found; i++)
				{

					if (invaders.at(i).x == x)
					{
						found = true;
						invaders.at(i).kill(tileMap);
						kill(tileMap);
						score++;
					}

				}
			}
			break;


		case 'X':
		case 'x':
			kill(tileMap);
			tileMap[y + yDir][x] = ' ';
			break;
		case '#':
			kill(tileMap);
			break;
		case '^':
			if (enemyBullet){
				game_running = false;
				break;
			}
		default:
			if (tileMap[y][x] == icon)
				tileMap[y][x] = ' ';
			y += yDir;
			tileMap[y][x] = icon;
			break;

		}

	}

};

void FireBullet(vector<bulletObj> &bullets, invObj &invader){

	bool found = false;
	for (int i = 0; i < bullets.size() && !found; i++){

		if (!bullets.at(i).isActive){
			bullets.at(i).enemyBullet = true;
			bullets.at(i).isActive = true;
			bullets.at(i).x = invader.x;
			bullets.at(i).y = invader.y;
			bullets.at(i).icon = '*';
			found = true;
		}

	}


}

struct pacObj
{
	const char pacch = '^';
	int x = 4;
	int y = 6;
	void takestep(int xAmount, vector<string> &tileMap);
};

//sets cursor to position 0,0 .. no need to modify
BOOL gotoxy(const WORD x, const WORD y)
{
	COORD xy;
	xy.X = x;
	xy.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

void loadmap(vector<string>& map)
{

	ifstream mapFile;
	string mapName;

	do
	{
		cout << "Map file: ";
		getline(cin, mapName);
		mapFile.open(mapName);

	} while (!mapFile);


	while (getline(mapFile, mapName))
	{
		map.push_back(mapName);
	}

	return;

}

//init all elements related to the game (your initial location, aliens, map, etc)
void init(vector<string>& mymap, vector<invObj>& invaders, pacObj& pac)
{

	loadmap(mymap);

	for (int i = 0; i < INIT_INVADERS; i++)
	{
		invaders.at(i).y = 3;
		invaders.at(i).x = i + 1;

		mymap[invaders.at(i).y][invaders.at(i).x] = invaders.at(i).alien;
	}

	pac.y = mymap.size() - 2;
	pac.x = mymap.at(mymap.size() - 1).size() / 2;
	mymap[pac.y][pac.x] = pac.pacch;

	return;
}


//draws to the screen
void updateScreen(const vector<string> &mymap, int dead)
{
	gotoxy(0, 0);

	for (string line : mymap)
	{
		cout << line << endl;
	}

	cout << endl << endl << "You killed " << dead << " aliens." << endl;
	return;

}

//move invaders 1 step
void moveInvaders(vector<invObj> &invaders, vector<string> &tileMap, bool &game_running)
{

	bool canMove = true;
	static int timesHit = 0;

	for (int i = 0; i < invaders.size() && canMove; i++)
	{

		if (invaders.at(i).alive&& !invaders.at(i).CanMove(tileMap, game_running))
			canMove = false;
	}
	if (!canMove)
	{
		timesHit++;

		for (int i = 0; i < invaders.size(); i++)
		{

			invaders.at(i).moveright = !invaders.at(i).moveright;
		}
	}
	else

	{

		for (int i = 0; i < invaders.size(); i++)
			invaders.at(i).Move(tileMap);
		if (timesHit == 8)
		{
			timesHit = 0;

			for (int i = 0; i < invaders.size(); i++)
			{
				invaders.at(i).y++;
			}
		}

		for (int i = 0; i < invaders.size(); i++)
			invaders.at(i).WriteToMap(tileMap);
	}

}

//move you one step
void pacObj::takestep(int xAmount, vector<string> &tileMap)
{


	if (' ' == tileMap[y][x + xAmount])
	{
		tileMap[y][x] = ' ';
		x += xAmount;
		tileMap[y][x] = pacch;
	}

	return;
}


int main()
{
	const int RANDOM_FIRE = 200;


	int speed = 100;
	bool game_running = true;
	int score = 0;
	vector<string> mymap;
	vector <invObj> invaders(INIT_INVADERS);
	vector <bulletObj> enemyBullets(20);
	pacObj pac;
	bulletObj bullet;
	init(mymap, invaders, pac);

	random_device rd; //ensures new set of numbers every time
	default_random_engine generator(rd());
	uniform_int_distribution<int> distribution(1, RANDOM_FIRE / mymap.size());
	do
	{
		Sleep(speed);

		// generates number in the range 1..6
		for (invObj &in : invaders){
			if (in.alive){
				int fire = distribution(generator);

				if (fire == 1) {
					FireBullet(enemyBullets, in);
				}
			}
		}
		if (GetAsyncKeyState(VK_RIGHT))
		{
			pac.takestep(1, mymap);
		}

		if (GetAsyncKeyState(VK_LEFT))
		{
			pac.takestep(-1, mymap);
		}

		//speed
		if (GetAsyncKeyState(VK_DOWN) && speed > 1)
		{
			speed -= 10;
		}

		if (GetAsyncKeyState(VK_UP) && speed < 200)
		{
			speed += 10;
		}

		if (GetAsyncKeyState(VK_ESCAPE))
		{
			game_running = false;
		}

		if (GetAsyncKeyState(VK_SPACE) && !bullet.isActive)
		{

			bullet.isActive = true;
			bullet.x = pac.x;
			bullet.y = pac.y;
		}

		if (GetAsyncKeyState(VK_CONTROL))
		{

			FireBullet(enemyBullets, invaders.at(0));
		}
		moveInvaders(invaders, mymap, game_running);
		if (bullet.isActive)
			bullet.Step(mymap, invaders, score, game_running);
		for (bulletObj &b : enemyBullets){
			if (b.isActive)
				b.Step(mymap, invaders, score, game_running);
		}


		updateScreen(mymap, score);

	} while ((game_running == true) && (score != INIT_INVADERS));

	//    system("cls");
	cout << "\n\nGAME OVER";
	system("pause>nul");

	return 0;
}
