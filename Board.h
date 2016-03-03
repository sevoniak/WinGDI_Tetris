#pragma once
#include "Shape.h"
#include "Definitions.h"

class Board
{
	HINSTANCE hInst;
	RECT outlineRect;
	RECT boardRect;
	HBITMAP hBitmap;
	HBITMAP hBitGO;
	HFONT hFontMain;
	int filled[HORZELEMS][VERTELEMS];
	int counter;
	int gameoverRow;
	int points;
	int cleared;
	int level;
	int numLines;
	int clearRow[4];
	int stats[7];
	int topscores[5];
	int menuState;
	bool locked;

public:
	Board(void);
	Board(HINSTANCE hInstIn, RECT rcIn);
	~Board(void);

	void SetLocked(bool input);
	bool GetLocked();
	void TranslateShape(int tDirectionIn, int amountIn, Shape* pShapeIn);
	void RotateShape(int rDirectionIn, Shape* pShapeIn);
	void LockShape(Shape* pShapeIn);
	void DrawBoard(HDC hdcBack, HDC hdcMain);
	void DrawTitle(HDC hdcBack, HDC hdcMain, HWND hwnd, int& lastSound);
	void DrawScores(HDC hdcBack, HDC hdcScore, int scoreIn, int index);
	bool GameOver(UINT wDeviceID, MCI_GENERIC_PARMS mciGenParms);
	bool IsGameOver(POINT spawnPoint);
	bool DetectClear();
	bool AnimateClear(HDC hdcBack, HDC hdcMain);
	int GetLevel();
	void SetMenuState(int stateIn);
	int GetMenuState();
	void LoadScores();
	void SaveScores();
	int StringToInt(wstring stringIN);
};
