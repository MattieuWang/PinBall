#pragma once
#include "stdafx.h"
#include<math.h>
#include "brick.h"


#define BALL_HEIGHT 10
#define BALL_WIDTH 10

class Ball
{
public:
	Ball();
	void setBallRect(RECT rect);           //根据板pad的RECT位置，设置Ball的初始RECT，确定位置
	~Ball();
	void update_ball(HDC &hdc, HDC &hdcBall, RECT rect, Brick bricks[][5]);    //更新球的位置 未发射状态和发射状态
	void setShoot(bool);      //允许发射
	void moving(RECT rectPad, Brick bricks[][5]);  //球发射后的状态，和碰撞判断
	void setDirXY(int x, int y);   //设置小球的运行方向
	bool getShoot();    //获取发射状态
	int getX() { return posX; }    //获得x位置
	int getY() { return posY; }    //获得y位置
	int getLife() { return life; } //获得小球life值
	void initBall(RECT rect);      //初始化小球
	void touchWall();             //撞墙判断
	void touchPad(RECT);          //撞板判断
	void touchBlock(Brick bricks[10][5]);   //撞砖块判断
	RECT getRectBall();    //获得小球RECT
	int getSpeedX() { return speedX; }  //获得x方向速度
	int getSpeedY() { return speedY; }  //获得y方向速度
	void setBrickState(Brick&b);        //设置砖块状态  type==2 则减1 type==1 则消失
	void addBallScore(int s) { score += s; }  //增加获得的分数
	int getBallScore() { return score; }   //活动小球分数
	//

private:
	int posX;
	int posY;
	bool shoot;
	bool shoot_flag;
	bool reflect_flag;
	int speedX;
	int speedY;
	int dirX;
	int dirY;
	RECT rectBall;
	int life;
	int score;
};

Ball::~Ball() {}
bool Ball::getShoot()
{
	return shoot;
}
Ball::Ball()
{
	/*
	shoot = FALSE;
	shoot_flag = FALSE;
	speedX = 5;
	speedY = 5;
	life = 3;
	*/
}

void Ball::setBallRect(RECT rectPad)
{
	posX = rectPad.left + 77;
	posY = rectPad.top - 16;
	rectBall = { rectPad.left,rectPad.top,rectPad.left + 16,rectPad.top + 16 };
}
void Ball::initBall(RECT rectPad)
{
	posX = rectPad.left + 77;
	posY = rectPad.top - 16;
	//rectBall = { posX, posY, 16, 16 };
	life = 1;
	shoot = FALSE;
	shoot_flag = FALSE;
	reflect_flag = TRUE;
	speedX = 5;
	speedY = 5;
	score = 0;
	rectBall = { rectPad.left,rectPad.top,rectPad.left + BALL_WIDTH,rectPad.top + BALL_HEIGHT };
}

void Ball::setShoot(bool state)
{
	shoot = state;
}
void Ball::setDirXY(int x, int y)
{
	double dir = sqrt(x * x + y * y);
	if (x <= 0)
	{
		speedY = -dir / x * 5;
		speedX = dir / y * 5;
	}
	else
	{
		speedY = dir / x * 5;
		speedX = -dir / y * 5;
	}
	if (speedX <= 4 && speedY <= 4)
	{
		speedX++;
		speedY++;
	}
}
void Ball::moving(RECT rectPad, Brick bricks[][5])
{
	touchWall();
	touchPad(rectPad);
	touchBlock(bricks);
	if (rectBall.left < 20)
		rectBall.left = 21;
	if (rectBall.right > 580)
		rectBall.left = 580 - BALL_WIDTH - 1;
	if (rectBall.top < 60)
		rectBall.top = 61;
}
void Ball::touchWall()
{
	if (rectBall.left <= 20 || rectBall.right >= 580)
	{
		speedX = -speedX;
	}
	if (rectBall.top <= 60)
	{
		speedY = -speedY;
	}
}
void Ball::touchPad(RECT rectPad)
{
	if (reflect_flag&&rectBall.bottom > rectPad.top&& rectBall.right < rectPad.right && rectBall.left > rectPad.left)
	{
		speedY = -speedY;
		reflect_flag = FALSE;
	}
	else if (reflect_flag&&rectBall.bottom > rectPad.top&&rectBall.top < rectPad.top&&rectBall.left<rectPad.left&&rectBall.right>rectPad.left&&speedX>0)   //向右撞左上角
	{
		speedX = -speedX;
		speedY = -speedY;
	}
	else if (reflect_flag&&rectBall.bottom > rectPad.top&&rectBall.top < rectPad.top&&rectBall.right>rectPad.right&&rectBall.left < rectPad.right&&speedX < 0)   //向左撞右上角
	{
		speedX = -speedX;
		speedY = -speedY;
	}
	else if (reflect_flag&&rectBall.bottom > rectPad.top&& rectBall.right < rectPad.right && rectBall.left > rectPad.left + 75)
	{
		speedX += 1;
		speedY = -speedY;
	}
	else if (reflect_flag&&rectBall.bottom > rectPad.top&& rectBall.right < rectPad.right - 75 && rectBall.left > rectPad.left)
	{
		speedX -= 1;
		speedY = -speedY;
	}
	else if (reflect_flag&&rectBall.left<rectPad.right&&rectBall.right<rectPad.right + 30 && rectBall.right>rectPad.right&&rectBall.bottom>rectPad.top)
	{
		speedX = -speedX;
		reflect_flag = FALSE;
	}
	else if (reflect_flag && rectBall.right > rectPad.left&&rectBall.left > rectPad.left - 30 && rectBall.left<rectPad.left&&rectBall.bottom>rectPad.top)
	{
		speedX = -speedX;
		reflect_flag = FALSE;
	}

	if (abs(speedX) < 2)
		speedX = 2;

	if (!reflect_flag&&rectBall.bottom < rectPad.top)
	{
		reflect_flag = TRUE;
	}
}
void Ball::setBrickState(Brick&b)
{
	if (b.getType() == 2)
	{
		b.setType(1);
	}
	else
	{
		b.setShow(FALSE);
		score += b.getScore();
	}

}
void Ball::touchBlock(Brick bricks[10][5])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (bricks[i][j].getShow())
			{

				if (rectBall.top < bricks[i][j].getBrickRect().bottom&&rectBall.bottom>bricks[i][j].getBrickRect().bottom&&rectBall.bottom <= bricks[i][j].getBrickRect().bottom + BALL_HEIGHT + 5 && rectBall.left > bricks[i][j].getBrickRect().left&&rectBall.right < bricks[i][j].getBrickRect().right)
				{
					speedY = -speedY;
					setBrickState(bricks[i][j]);
				}
				else if (rectBall.bottom > bricks[i][j].getBrickRect().top&&rectBall.top < bricks[i][j].getBrickRect().top - BALL_HEIGHT + 5 && rectBall.left > bricks[i][j].getBrickRect().left&&rectBall.right < bricks[i][j].getBrickRect().right)
				{
					speedY = -speedY;
					setBrickState(bricks[i][j]);
				}

				else if (rectBall.right >= bricks[i][j].getBrickRect().left&&rectBall.left < bricks[i][j].getBrickRect().left - BALL_WIDTH + 1 && rectBall.bottom < bricks[i][j].getBrickRect().bottom&&rectBall.top > bricks[i][j].getBrickRect().top&&speedX>0)
				{
					//speedX++;
					speedX = -speedX;
					setBrickState(bricks[i][j]);
				}

				else if (rectBall.left <= bricks[i][j].getBrickRect().right&&rectBall.right > bricks[i][j].getBrickRect().right + BALL_WIDTH - 1 && rectBall.bottom < bricks[i][j].getBrickRect().bottom&&rectBall.top > bricks[i][j].getBrickRect().top&&speedX < 0)
				{
					speedX = -speedX;
					setBrickState(bricks[i][j]);
				}

				//撞左下角
				else if (rectBall.top < bricks[i][j].getBrickRect().bottom&&rectBall.bottom>bricks[i][j].getBrickRect().bottom&&rectBall.left< bricks[i][j].getBrickRect().left&&rectBall.right>bricks[i][j].getBrickRect().left)
				{
					if (speedX > 0)
					{
						speedX = -speedX;
						speedY = -speedY;
					}
					else
						speedY = -speedY;
					setBrickState(bricks[i][j]);

				}
				//撞左上角
				else if (rectBall.bottom > bricks[i][j].getBrickRect().top&&rectBall.top < bricks[i][j].getBrickRect().top&&rectBall.left< bricks[i][j].getBrickRect().left&&rectBall.right>bricks[i][j].getBrickRect().left)
				{
					if (speedX > 0)
					{
						speedX = -speedX;
						speedY = -speedY;
					}
					else
						speedY = -speedY;
					setBrickState(bricks[i][j]);
				}
				//撞右下角
				else if (rectBall.top < bricks[i][j].getBrickRect().bottom&&rectBall.bottom>bricks[i][j].getBrickRect().bottom&&rectBall.left< bricks[i][j].getBrickRect().right&&rectBall.right>bricks[i][j].getBrickRect().right)
				{
					if (speedX < 0)
					{
						speedX = -speedX;
						speedY = -speedY;
					}
					else
						speedY = -speedY;
					setBrickState(bricks[i][j]);
				}
				//撞右上角
				else if (rectBall.bottom > bricks[i][j].getBrickRect().top&&rectBall.top < bricks[i][j].getBrickRect().top&&rectBall.left< bricks[i][j].getBrickRect().right&&rectBall.right>bricks[i][j].getBrickRect().right)
				{
					if (speedX < 0)
					{
						speedX = -speedX;
						speedY = -speedY;
					}
					else
						speedY = -speedY;
					setBrickState(bricks[i][j]);
				}
			}
		}
	}
}



void Ball::update_ball(HDC &hdc, HDC &hdcBall, RECT rectPad, Brick bricks[][5])
{
	if (!shoot&&life > 0)
	{
		rectBall.left = rectPad.left + 60;
		rectBall.top = rectPad.top - BALL_HEIGHT;
	}
	else
	{
		moving(rectPad, bricks);
		rectBall.left += speedX;
		rectBall.top -= speedY;
		if (rectBall.top > rectPad.bottom + 50)
		{
			shoot = FALSE;

			if (life > 0)
			{
				life--;
				reflect_flag = TRUE;
			}
		}
	}
	//TransparentBlt(hdc, posX, posY, 16, 16, hdcBall, 0, 0, 16, 16, RGB(255, 255, 255));
	TransparentBlt(hdc, rectBall.left, rectBall.top, BALL_WIDTH, BALL_HEIGHT, hdcBall, 0, 0, 219, 217, RGB(22, 22, 22));
	rectBall.right = rectBall.left + BALL_HEIGHT;
	rectBall.bottom = rectBall.top + BALL_WIDTH;
}

RECT Ball::getRectBall()
{
	return rectBall;
}

