#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <random>

#include "windows.h"
#include "Invader.h"
#include "Pacman.h"
#include "Bullet.h"

using namespace std;

const int INIT_INVADERS = 5;

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
		for (invObj &in : invaders) {
			if (in.alive) {
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
		for (bulletObj &b : enemyBullets) {
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
