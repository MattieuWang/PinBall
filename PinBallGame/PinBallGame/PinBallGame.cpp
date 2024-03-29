// PinBallGame.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PinBallGame.h"
#include "pad.h"
#include "ball.h"
#include "brick.h"
#include<string>
#include<fstream>
#include<iostream>
using namespace std;

#pragma comment(lib,"msimg32.lib")
#define MAX_LOADSTRING 100


HDC g_hdc;
HDC g_hdcBuf;
HDC g_hdcBuf2;
HDC g_hdcBg;
HDC g_hdcBegin;
HDC g_hdcBall;
HDC g_hdcPad;
HDC g_hdcTest;
HDC g_hdcgameOver1;
HDC g_hdcgameOver2;
HDC g_hdcBrick1;
HDC g_hdcBrick2;
HDC g_hdcGuangBiao;

HBITMAP g_hBmp;

int guangBiao = 0;
Brick bricks[10][5];
int guangBiaoPos = 463;



Pad pad;
Ball ball;
int game_state = 0;
int typ[50];

static POINT ptBeg, ptEnd;


// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void load_game(HWND);
void paint_game();
void init_game();
void game_over();
void paint_text();



//设置50块砖块的种类

void setBricks()
{
	int types[50];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			if ((i + j) % 2 == 1)
			{
				bricks[i][j].setType(1);
				bricks[i][j].setScore(100);
			}
			
			else
			{
				bricks[i][j].setType(2);
				bricks[i][j].setScore(300);
			}
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_PINBALLGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINBALLGAME));

	MSG msg;
	init_game();
	DWORD tPre = 0, tCur = 0;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		else //没有窗口消息
		{
			tCur = GetTickCount();
			if (tCur - tPre > 5)  //屏幕刷新间隔大于50ms
			{
				tPre = tCur;
				paint_game();
			}
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PINBALLGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;// MAKEINTRESOURCEW(IDC_PINBALLGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW&~WS_THICKFRAME&~WS_MAXIMIZEBOX,
		400, 10, 600 + 16, 800, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	load_game(hWnd);
	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
		ptBeg.x = LOWORD(lParam);
		ptBeg.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		if (!ball.getShoot())
		{
			ptEnd.x = LOWORD(lParam);
			ptEnd.y = HIWORD(lParam);
			if (abs(ptBeg.x - ptEnd.x) > 30 && abs(ptBeg.y - ptEnd.y > 30))
			{
				ball.setDirXY(ptBeg.x - ptEnd.x, ptBeg.y - ptEnd.y);
				ball.setShoot(TRUE);
			}
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			pad.setMovingLeft(TRUE);
			break;
		case VK_RIGHT:
			pad.setMovingRight(TRUE);
			break;
		case VK_RETURN:
			if (game_state == 0&&guangBiaoPos==463)
				game_state++;
			if (game_state == 0 && guangBiaoPos == 571)
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			if (game_state == 2 && guangBiao == 0)
			{
				game_state = 1;
				guangBiao = 0;
				init_game();
			}
			if (game_state == 2 && guangBiao == 1)
			{
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			}
			break;
		case 'Q':
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
				break;
		case VK_DOWN:
			if (game_state == 2 && guangBiao == 0)
			{
				guangBiao++;
				game_over();
			}
			if (game_state == 0)
			{
				if (guangBiaoPos == 463)
				{
					guangBiaoPos = 571;
				}
			}
			break;
		case VK_UP:
			if (game_state == 2 && guangBiao == 1)
			{
				guangBiao--;
				game_over();
			}
			if (game_state == 0)
			{
				if (guangBiaoPos == 571)
				{
					guangBiaoPos = 463;
				}
			}
			break;
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			pad.setMovingLeft(FALSE);
			break;
		case VK_RIGHT:
			pad.setMovingRight(FALSE);
		default:
			break;
		}
		break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}




void init_game()
{

	pad.setPos(223, 650);
	//ball.setBallRect(pad.getPadRect());
	ball.initBall(pad.getPadRect());
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			bricks[i][j].setPos(55 * i + 25, 63 + j * 20);
			bricks[i][j].setShow(TRUE);
		}
	}
	setBricks();
}


void load_image(HDC &hdc, TCHAR szFileName[])
{
	HBITMAP hBmp;
	hBmp = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hdc = CreateCompatibleDC(g_hdc);
	SelectObject(hdc, hBmp);
	DeleteObject(hBmp);
}


void load_game(HWND hWnd)
{
	g_hdc = GetDC(hWnd);
	g_hdcBuf = CreateCompatibleDC(g_hdc);
	g_hBmp = CreateCompatibleBitmap(g_hdc, 600, 800);
	SelectObject(g_hdcBuf, g_hBmp);
	load_image(g_hdcBg, (_TCHAR*)L"image\\playing.bmp");
	load_image(g_hdcBall, (_TCHAR*)L"image\\ball.bmp");
	load_image(g_hdcPad, (_TCHAR*)L"image\\pad.bmp");
	load_image(g_hdcBegin, (_TCHAR*)L"image\\beginBg.bmp");
	load_image(g_hdcgameOver1, (_TCHAR*)L"image\\gameOver1.bmp");
	load_image(g_hdcgameOver2, (_TCHAR*)L"image\\gameOver2.bmp");
	load_image(g_hdcBrick1, (_TCHAR*)L"image\\brick1fois.bmp");
	load_image(g_hdcBrick2, (_TCHAR*)L"image\\brick2fois.bmp");
	load_image(g_hdcGuangBiao, (_TCHAR*)L"image\\guangbiao.bmp");
}

void paint_game()
{
	if (game_state == 0)
	{
		init_game();
	
		BitBlt(g_hdcBuf, 0, 0, 600, 800, g_hdcBegin, 0, 0, SRCCOPY);
		BitBlt(g_hdc, 0, 0, 600, 800, g_hdcBuf, 0, 0, SRCCOPY);
		TransparentBlt(g_hdc, 165, guangBiaoPos, 244, 25, g_hdcGuangBiao, 0, 0, 244, 25, RGB(255, 255, 255));
	}
	if (game_state == 1)
	{
		BitBlt(g_hdcBuf, 0, 0, 600, 800, g_hdcBg, 0, 0, SRCCOPY);

		paint_text();
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				if (bricks[i][j].getType() == 1)
					bricks[i][j].update_Brick(g_hdcBuf, g_hdcBrick1);
				else if (bricks[i][j].getType() == 2)
					bricks[i][j].update_Brick(g_hdcBuf, g_hdcBrick2);
			}
		}

		pad.update_pad(g_hdcBuf, g_hdcPad);
		BitBlt(g_hdc, 0, 0, 600, 800, g_hdcBuf, 0, 0, SRCCOPY);

		ball.update_ball(g_hdc, g_hdcBall, pad.getPadRect(), bricks);

	}
	if (ball.getLife() == 0)
	{
		game_state = 2;

		game_over();
		paint_text();
		BitBlt(g_hdc, 0, 0, 600, 800, g_hdcBuf, 0, 0, SRCCOPY);
	}

}

void game_over()
{
	if (guangBiao == 0)
	{
		BitBlt(g_hdcBuf, 0, 0, 600, 800, g_hdcgameOver1, 0, 0, SRCCOPY);
	}
	if (guangBiao == 1)
	{
		BitBlt(g_hdcBuf, 0, 0, 600, 800, g_hdcgameOver2, 0, 0, SRCCOPY);
	}

}

void paint_text()
{
	RECT rect1 = { 120,20,100,100 };
	RECT rect2 = { 450,20,550,100 };
	RECT rect3 = { 150,330,300,450 };
	TCHAR szTip1[10];
	TCHAR szTip2[20];
	TCHAR szTip3[20];
	wsprintf(szTip1, L"%4d", ball.getLife());
	wsprintf(szTip2, L"SCORE: %4d", ball.getBallScore());
	wsprintf(szTip3, L"%4d", ball.getBallScore());
	if (ball.getLife() > 0)
	{

		//int flag = ball.getShoot()? 1: 0;
		BitBlt(g_hdcBuf, 0, 0, 600, 800, g_hdcBg, 0, 0, SRCCOPY);
		SetBkMode(g_hdcBuf, TRANSPARENT);
		SetTextColor(g_hdcBuf, RGB(255, 204, 0));

		DrawText(g_hdcBuf, szTip1, -1, &rect1, DT_CENTER);
		DrawText(g_hdcBuf, szTip2, -1, &rect2, DT_CENTER);
	}
	
	else
	{
		SetTextColor(g_hdcBuf, RGB(255, 255, 255));
		DrawText(g_hdcBuf, szTip3, -1, &rect3, DT_CENTER);
	}
	
}