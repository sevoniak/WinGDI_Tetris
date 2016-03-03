#include "Board.h"
#define STATE_IDLE 0
#define STATE_INIT 1
#define STATE_TITLE 2
#define STATE_RUN 3
#define STATE_CLEAR 4
#define STATE_GAMEOVER 5
#define STATE_QUIT 6

HINSTANCE hInst;
HDC hdcMain;
HDC hdcBack;
HBITMAP hBitBack;
int cxClient, cyClient;
int gamestate = STATE_IDLE;
int lastSound = 0;
bool loopMusic = false;
bool doOnce = false;
Board* pBoardGlobal = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GameMain(HINSTANCE hInst, HWND hwnd);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	const wchar_t CLASS_NAME[] = L"TetrisClass";
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
//	wc.hIconSm =
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	HWND hwnd = CreateWindow(CLASS_NAME, L"Tetris", WS_OVERLAPPEDWINDOW &~ WS_MAXIMIZEBOX &~ WS_THICKFRAME, CW_USEDEFAULT, 20,
							 500, 500, NULL, NULL, hInstance, NULL);
	if (!hwnd)
		return 0;
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg = {};

	bool running = true;
	while (running)
	{
		DWORD start_tick = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				running = false;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		GameMain (hInstance, hwnd);
		BitBlt(hdcMain, 0, 0, 750, 500, hdcBack, 0, 0, SRCCOPY);
		while (GetTickCount() - start_tick < GAMESPEED);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CREATE:
		return 0;

	case WM_CLOSE:
		gamestate = STATE_QUIT;
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		if (gamestate == STATE_IDLE)
			gamestate = STATE_INIT;
		return 0;

	case WM_KEYDOWN:
		return 0;

	case WM_LBUTTONUP:
		{	
			POINT mousePos;
			mousePos.x = LOWORD(lParam);
			mousePos.y = HIWORD(lParam);

			switch (pBoardGlobal->GetMenuState())
			{
			case 0:
				{
					if (mousePos.x > 200 && mousePos.x < 300)
						if (mousePos.y > 250 && mousePos.y < 290)
						{
							doOnce = true;
							gamestate = STATE_RUN;
							lastSound = 0;
						}
					if (mousePos.x > 165 && mousePos.x < 340)
						if (mousePos.y > 300 && mousePos.y < 335)
						{
							pBoardGlobal->SetMenuState(1);
							lastSound = 0;
						}
					if (mousePos.x > 150 && mousePos.x < 355)
						if (mousePos.y > 350 && mousePos.y < 385)
						{
							pBoardGlobal->SetMenuState(2);
							lastSound = 0;
						}
					break;
				}
			case 1:
				if (mousePos.x > 205 && mousePos.x < 285)
					if (mousePos.y > 400 && mousePos.y < 435)
					{
						pBoardGlobal->SetMenuState(0);
						lastSound = 0;
					}
				break;
			case 2:
				if (mousePos.x > 195 && mousePos.x < 270)
					if (mousePos.y > 410 && mousePos.y < 445)
					{
						pBoardGlobal->SetMenuState(0);
					}
				break;
			default: break;
			}
			return 0;
		}

	case MM_MCINOTIFY:  
		if (wParam == MCI_NOTIFY_SUCCESSFUL)
			loopMusic = true;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void GameMain (HINSTANCE hInst, HWND hwnd)
{
	static Board* pBoard;
	static Shape* pCurrShape;
	static Shape* pNextShape;
	static int shape, count, level;
	static POINT spawnPoint, nextPoint;
	static RECT rc, clientRect;
	static UINT wDeviceID;
	static MCI_OPEN_PARMS mciOpenParms;
    static MCI_PLAY_PARMS mciPlayParms;
	static MCI_GENERIC_PARMS mciGenParms;

	switch (gamestate)
	{
	case STATE_IDLE:
		break;

	case STATE_INIT:
		srand((int)time(0));
		hdcMain = GetDC(hwnd);
		hdcBack = CreateCompatibleDC(hdcMain);
		hBitBack = CreateCompatibleBitmap(hdcMain, 500, 500);
		SelectObject(hdcBack, hBitBack);
		mciGenParms.dwCallback = (DWORD) hwnd;
		mciOpenParms.lpstrDeviceType = L"waveaudio";
		mciOpenParms.lpstrElementName = L"title.wav";
		mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms);
		wDeviceID = mciOpenParms.wDeviceID;
		mciPlayParms.dwCallback = (DWORD) hwnd;

		doOnce = false;
		count = 0;
		rc.top = 25;
		rc.bottom = rc.top + BOARDHEIGHT;
		rc.left = (cxClient - BOARDWIDTH) / 2;
		rc.right = rc.left + BOARDWIDTH;
		GetClientRect(hwnd, &clientRect);
		spawnPoint.x = 4;
		spawnPoint.y = 0;
		nextPoint.x = 50;
		nextPoint.y = 150;
		pBoard = new Board(hInst, rc);
		pBoardGlobal = pBoard;
		pBoard->LoadScores();
		pCurrShape = 0;
		pNextShape = 0;
		level = pBoard->GetLevel();
		mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD_PTR) &mciPlayParms);
		gamestate = STATE_TITLE;
		break;

	case STATE_TITLE:
		if (loopMusic)
		{
			mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD_PTR) &mciPlayParms);
			loopMusic = false;
		}
		FillRect(hdcBack, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		pBoard->DrawTitle(hdcBack, hdcMain, hwnd, lastSound);
		break;

	case STATE_RUN:
		{
			if (doOnce)
			{
				shape = rand() % 7;
				pNextShape = new Shape(hInst, shape, nextPoint);
				loopMusic = true;
				mciSendCommand(wDeviceID, MCI_STOP, NULL, (DWORD_PTR) &mciGenParms);
				mciSendCommand(wDeviceID, MCI_CLOSE, NULL, (DWORD_PTR) &mciGenParms);
				mciOpenParms.lpstrElementName = L"bgm.wav";
				mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms);
				wDeviceID = mciOpenParms.wDeviceID;
				doOnce = false;
			}
			int tDirection = 0;
			int rDirection = 0;
			if (loopMusic)
			{
				mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD_PTR) &mciPlayParms);
				loopMusic = false;
			}
			if (pBoard->IsGameOver(spawnPoint))
			{
				gamestate = STATE_GAMEOVER;
				break;
			}
			if (pCurrShape == 0)
			{
				shape = pNextShape->GetType();
				pCurrShape = new Shape(hInst, shape, spawnPoint);
				delete(pNextShape);
				shape = rand() % 7;
				if (shape == pCurrShape->GetType())
				{
					if (rand()%2 == 0)
						shape = rand() % 7;
				}
				pNextShape = new Shape(hInst, shape, nextPoint);
				pBoard->SetLocked(false);
			}
			if (!count)
			{
				if (GetAsyncKeyState(VK_RIGHT))
				{
					tDirection = 1;
					pBoard->TranslateShape(tDirection, 1,pCurrShape);
				}
				if (GetAsyncKeyState(VK_LEFT))
				{
					tDirection = -1;
					pBoard->TranslateShape(tDirection, 1, pCurrShape);
				}
				if (GetAsyncKeyState(VK_DOWN))
				{
					pBoard->TranslateShape(tDirection, BLOCKSIZE, pCurrShape);
					if(pBoard->GetLocked())
					{
						delete pCurrShape;
						pCurrShape = 0;
					}
				}
				if (GetAsyncKeyState(VK_NUMPAD1))
				{
					rDirection = -1;
					pBoard->RotateShape(rDirection, pCurrShape);
				}
				if (GetAsyncKeyState(VK_NUMPAD3))
				{
					rDirection = 1;
					pBoard->RotateShape(rDirection, pCurrShape);
				}
			}
			count++;
			if (count == 2)
				count = 0;

			if (pCurrShape != 0)
			{
				pBoard->TranslateShape(0, level + STEPS - 1, pCurrShape);
				if(pBoard->GetLocked())
				{
					delete pCurrShape;
					pCurrShape = 0;
				}
			}

			FillRect(hdcBack, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			pBoard->DrawBoard(hdcBack, hdcMain);
			if (pCurrShape != 0)
			{
				pCurrShape->Print(hdcBack, hdcMain, rc);
				pNextShape->Print(hdcBack, hdcMain);
			}

			if (pBoard->DetectClear())
				gamestate = STATE_CLEAR;
			break;
		}

	case STATE_CLEAR:
		{
			FillRect(hdcBack, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			bool more = pBoard->AnimateClear(hdcBack, hdcMain);
			pNextShape->Print(hdcBack, hdcMain);
			if(!more)
			{
				level = pBoard->GetLevel();
				gamestate = STATE_RUN;
			}
			break;
		}

	case STATE_GAMEOVER:
		FillRect(hdcBack, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
		pBoard->DrawBoard(hdcBack, hdcMain);
		if (pBoard->GameOver(wDeviceID, mciGenParms))
		{
			MessageBox(hwnd, L"Game Over", L"Game Over", MB_OK);
			pBoard->SaveScores();
			delete pBoard;
			pBoard = new Board(hInst, rc);
			pBoardGlobal = pBoard;
			pBoard->LoadScores();
			delete pCurrShape;
			pCurrShape = 0;
			delete pNextShape;
			pNextShape = 0;
			mciSendCommand(wDeviceID, MCI_CLOSE, NULL, (DWORD_PTR) &mciGenParms);
			mciOpenParms.lpstrElementName = L"title.wav";
			mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &mciOpenParms);
			wDeviceID = mciOpenParms.wDeviceID;
			loopMusic = true;
			gamestate = STATE_TITLE;
		}
		break;

	case STATE_QUIT:
		DeleteObject(hBitBack);
		DeleteDC(hdcBack);
		ReleaseDC(hwnd, hdcMain);
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, NULL);
		gamestate++;
		DestroyWindow(hwnd);
		break;
	default: break;
	}
	return;
}