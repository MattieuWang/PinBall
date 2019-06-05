#pragma once
#include"stdafx.h"
class Brick
{
public:
	Brick();
	Brick(int x, int y);
	void setPos(int x, int y);
	int getX();
	RECT getBrickRect();
	void update_Brick(HDC &hdc, HDC &hdcBrick);
	void setShow(bool s) { show = s; }
	bool getShow() { return show; }
	void setType(int a) { type = a; }
	int getType() { return type; };
	void setScore(int s) { score = s; }
	int getScore() { return score; }

	~Brick();


private:
	int posX;
	int posY;
	bool show;
	int type;
	RECT BrickRect;
	int score;
};

Brick::Brick()
{
	show = TRUE;
}


Brick::~Brick()
{
}

Brick::Brick(int x, int y)
{
	posX = x;
	posY = y;

}

void Brick::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

int Brick::getX()
{
	return posX;
}

void Brick::update_Brick(HDC & hdc, HDC & hdcBrick)
{
	//TransparentBlt(hdc, posX, posY,55, 20, hdcBrick, 0, 0, 55, 20, RGB(255, 255, 255));
	if (show)
		BitBlt(hdc, posX, posY, 55, 20, hdcBrick, 0, 0, SRCCOPY);
	//(hdc, 0, 0, 600, 800, hdcBuf, 0, 0, SRCCOPY);
	BrickRect = { posX,posY,posX + 55,posY + 20 };
}

RECT Brick::getBrickRect()
{
	return BrickRect;
}