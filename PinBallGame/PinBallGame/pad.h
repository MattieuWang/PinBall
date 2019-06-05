#include "stdafx.h"
#define PAD_WIDTH 136

class Pad
{
public:
	Pad();
	Pad(int x, int y);   //pad��ʼ��
	void setPos(int x, int y);  //����padλ��
	int getX();  //���xλ��
	RECT getPadRect();  //���pad��RECTֵ
	void update_pad(HDC &hdc, HDC &hdcPad);  //����pad��λ��
	void setMovingLeft(bool);  //�����Ƿ����������ƶ�
	void setMovingRight(bool);  //�����Ƿ����������ƶ�
	~Pad();

private:
	int posX;
	int posY;
	int speed;
	bool moving_left;
	bool moving_right;
	RECT padRect;
};

Pad::Pad()
{
	speed = 5;
	moving_left = FALSE;
	moving_right = FALSE;
}

Pad::~Pad()
{
}

Pad::Pad(int x, int y)
{
	posX = x;
	posY = y;
	speed = 10;
	moving_left = FALSE;
	moving_right = FALSE;
}

void Pad::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

int Pad::getX()
{
	return posX;
}
void Pad::update_pad(HDC & hdc, HDC & hdcPad)
{
	if (moving_left)
	{
		if (posX > 20)
		{
			posX -= speed;
		}
	}
	if (moving_right)
	{
		if (posX+135<580)
		{
			posX += speed;
		}
	}
	//TransparentBlt(hdc, posX, posY, 135, 14, hdcPad, 0, 0, 135, 14, RGB(255, 255, 255));
	BitBlt(hdc, posX, posY, 135, 14, hdcPad, 0, 0, SRCCOPY);
	padRect = { posX, posY, posX+135, posY+14 };
}

RECT Pad::getPadRect()
{
	return padRect;
}

void Pad::setMovingLeft(bool ml)
{
	moving_left = ml;
}

void Pad::setMovingRight(bool mr)
{
	moving_right = mr;
}