#ifndef INVADER_H
#define INVADER_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class invObj
{
public:
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

#endif
