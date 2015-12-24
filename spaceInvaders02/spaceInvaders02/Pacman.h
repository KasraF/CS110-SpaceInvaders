#ifndef PACMAN_H
#define PACMAN_H

class pacObj
{
public:
	const char pacch = '^';
	int x = 4;
	int y = 6;
	void takestep(int xAmount, vector<string> &tileMap);
};

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


#endif