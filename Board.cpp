#include "Board.h"

Board::Board(void)
{
}

Board::Board(HINSTANCE hInstIn, RECT rcIn):
hInst(hInstIn),
boardRect(rcIn)
{
	menuState = 0;
	counter = 0;
	points = 0;
	cleared = 0;
	level = 1;
	numLines = 0;
	for (int i = 0; i < 4; i++)
		clearRow[i] = -1;
	for (int i = 0; i < 7; i ++)
		stats[i] = 0;
	for (int i = 0; i < 5; i++)
		topscores[i] = 0;
	gameoverRow = VERTELEMS - 1;
	locked = false;
	outlineRect.left = boardRect.left - 1;
	outlineRect.right = boardRect.right + 1;
	outlineRect.top = boardRect.top - 1;
	outlineRect.bottom = boardRect.bottom + 1;
	for (int i = 0; i < HORZELEMS; i++)
		for (int j = 0; j < VERTELEMS; j++)
			filled[i][j] = -1;
	hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0,0,0);
	hBitGO = (HBITMAP) LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, 0);
	hFontMain = CreateFont(18,0,0,0,0,0,0,0,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,0,0,0,L"Arial");
}

Board::~Board(void)
{
	DeleteObject(hBitmap);
	DeleteObject(hBitGO);
	DeleteObject(hFontMain);
}

void Board::SetLocked(bool input) { locked = input; return; }

bool Board::GetLocked() { return locked; }

void Board::TranslateShape(int tDirectionIn, int amountIn, Shape* pShapeIn)
{
	POINT* pt = pShapeIn->GetPoints();
	POINT* pLoc = pt;
	int row = 0;
	int column = 0;

	switch (tDirectionIn)
	{
	case -1:
		for (int i = 0; i < 4; i++)
		{
			row = pLoc->y / BLOCKSIZE;
			column = pLoc->x;
			if (row > -1)
			{
				if (column == 0 || filled[column-1][row] > -1)
					return;
				if (pLoc->y % BLOCKSIZE > 0)
				{
					if (filled[pLoc->x-1][row+1] > -1)
						return;
				}
			}
			pLoc++;
		}
		break;
	case 0:
		for (int i = 0; i < 4; i++)
		{
			row = pLoc->y / BLOCKSIZE;
			column = pLoc->x;
			if (row > -1)
			{
				int newY = pLoc->y + amountIn;
				int destRow = newY / BLOCKSIZE;
				if (filled[column][destRow+1] > -1 || destRow == (VERTELEMS -1))
				{
					amountIn = BLOCKSIZE * destRow - pLoc->y;
					pShapeIn->Translate(tDirectionIn, amountIn);
					LockShape(pShapeIn);
					return;
				}
			}
			pLoc++;
		}
		break;
	case 1:
		for (int i = 0; i < 4; i++)
		{
			row = pLoc->y / BLOCKSIZE;
			column = pLoc->x;
			if (row > -1)
			{
				if (column == HORZELEMS-1 || filled[column+1][row] > -1)
					return;
				if (pLoc->y % BLOCKSIZE > 0)
				{
					if (filled[column+1][row+1] > -1)
						return;
				}
			}
			pLoc++;
		}
		break;
	default: break;
	}
	pShapeIn->Translate(tDirectionIn, amountIn);
	return;
}

void Board::RotateShape(int rDirectionIn, Shape* pShapeIn)
{
	int currConfig = 0;
	int nextConfig = 0;
	ConfigSet* pConfig = pShapeIn->GetConfig(currConfig);
	ConfigSet* pCLoc = pConfig;
	POINT* pt = pShapeIn->GetPoints();

	nextConfig = currConfig + rDirectionIn;
	if (nextConfig == 4)
		nextConfig = 0;
	if (nextConfig == -1)
		nextConfig = 3;
	for (int i = 0; i < nextConfig; i++)
		pCLoc++;

	POINT testPoint;
	testPoint.x = pt->x + pCLoc->p1.x;
	testPoint.y = pt->y + pCLoc->p1.y * BLOCKSIZE;
	if (testPoint.x < 0 || testPoint.x > HORZELEMS - 1 || testPoint.y / BLOCKSIZE > VERTELEMS-1)
		return;
	if (filled[testPoint.x][testPoint.y / BLOCKSIZE] > -1)
		return;
	if (testPoint.y % BLOCKSIZE > 0)
		if (filled[testPoint.x][testPoint.y / BLOCKSIZE +1] > -1)
			return;
	testPoint.x = pt->x + pCLoc->p2.x;
	testPoint.y = pt->y + pCLoc->p2.y * BLOCKSIZE;
	if (testPoint.x < 0 || testPoint.x > HORZELEMS - 1 || testPoint.y / BLOCKSIZE > VERTELEMS-1)
		return;
	if (filled[testPoint.x][testPoint.y / BLOCKSIZE] > -1)
		return;
	if (testPoint.y % BLOCKSIZE > 0)
		if (filled[testPoint.x][testPoint.y / BLOCKSIZE +1] > -1)
			return;
	testPoint.x = pt->x + pCLoc->p3.x;
	testPoint.y = pt->y + pCLoc->p3.y * BLOCKSIZE;
	if (testPoint.x < 0 || testPoint.x > HORZELEMS - 1 || testPoint.y / BLOCKSIZE > VERTELEMS-1)
		return;
	if (filled[testPoint.x][testPoint.y / BLOCKSIZE] > -1)
		return;
	if (testPoint.y % BLOCKSIZE > 0)
		if (filled[testPoint.x][testPoint.y / BLOCKSIZE +1] > -1)
			return;
	
	pShapeIn->Rotate(rDirectionIn);
	return;
}

void Board::LockShape(Shape* pShapeIn)
{
	POINT* pt = pShapeIn->GetPoints();
	POINT* pLoc = pt;
	int column = 0;
	int row = 0;

	for (int i = 0; i < 4; i++)
	{
		column = pLoc->x;
		row = pLoc->y / BLOCKSIZE;
		if (row > -1)
			filled[column][row] = pShapeIn->GetType();
		pLoc++;
	}
	locked = true;
	PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
	stats[pShapeIn->GetType()]++;
	points += 5;
	return;
}

void Board::DrawBoard(HDC hdcBack, HDC hdcMain)
{
	Rectangle(hdcBack, outlineRect.left, outlineRect.top, outlineRect.right, outlineRect.bottom);

	HDC hdcBitmap = CreateCompatibleDC(hdcMain);
	SelectObject(hdcBitmap, hBitmap);

	for (int i = 0; i < HORZELEMS; i++)
		for (int j = 0; j < VERTELEMS; j++)
			if (filled[i][j] >= 0)
				BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+j*BLOCKSIZE, BLOCKSIZE, BLOCKSIZE, hdcBitmap, filled[i][j] * BLOCKSIZE, 0, SRCCOPY);
	
	HFONT oldFont = (HFONT) SelectObject(hdcBack, hFontMain);
	Rectangle(hdcBack, 5, 5, 130, 60);
	wstring out1 = L"Points";
	TextOut(hdcBack, 10, 10, out1.c_str(), out1.length());
	wstringstream ss1;
	ss1 << points;
	TextOut(hdcBack, 35, 35, ss1.str().c_str(), ss1.str().length());
	Rectangle(hdcBack, 5, 105, 130, 200);
	wstring out2 = L"Next";
	TextOut(hdcBack, 10, 110, out2.c_str(), out2.length());
	Rectangle(hdcBack, 5, 235, 130, 360);
	wstring out3 = L"Level";
	wstring out4 = L"Lines";
	TextOut(hdcBack, 10, 240, out3.c_str(), out3.length());
	TextOut(hdcBack, 10, 300, out4.c_str(), out4.length());
	wstringstream ss3, ss4;
	ss3 << level;
	ss4 << cleared;
	TextOut(hdcBack, 35, 265, ss3.str().c_str(), ss3.str().length());
	TextOut(hdcBack, 35, 325, ss4.str().c_str(), ss4.str().length());
	Rectangle (hdcBack, 360, 10, 490, 310);
	wstring out5 = L"Pieces Played";
	TextOut(hdcBack, 365, 15, out5.c_str(), out5.length());
	for (int i = 0; i < 7; i++)
	{
		BitBlt(hdcBack, 375, 40 + 35*i, BLOCKSIZE, BLOCKSIZE, hdcBitmap, i*20, 0, SRCCOPY);
		wstringstream ssStats;
		ssStats << L"-  " << stats[i];
		TextOut(hdcBack, 410, 40 + 35*i, ssStats.str().c_str(), ssStats.str().length());
	}
	wstring out6 = L"Total:";
	TextOut(hdcBack, 400, 285, out6.c_str(), out6.length());
	wstringstream ss6;
	int total = 0;
	for (int i = 0; i < 7; i++)
		total += stats[i];
	ss6 << total;
	TextOut(hdcBack, 455, 285, ss6.str().c_str(), ss6.str().length());

	SelectObject(hdcBack, oldFont);
	DeleteDC(hdcBitmap);

	if (gameoverRow < 0)
	{
		HDC hdcGO = CreateCompatibleDC(hdcMain);
		SelectObject(hdcGO, hBitGO);
		BitBlt(hdcBack, 100, 130, 287, 153, hdcGO, 0, 0, SRCCOPY);
		DeleteDC(hdcGO);
	}
	return;
}

void Board::DrawTitle(HDC hdcBack, HDC hdcMain, HWND hwnd, int& lastSound)
{
	HDC hdcBitmap = CreateCompatibleDC(hdcMain);
	HDC hdcTitles = CreateCompatibleDC(hdcMain);
	HDC hdcCont = CreateCompatibleDC(hdcMain);
	HDC hdcScore = CreateCompatibleDC(hdcMain);

	HBITMAP hBitTitle = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0,0,0);
	HBITMAP hBitCont = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0,0,0);
	HBITMAP hBitScore = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0,0,0);
	SelectObject(hdcBitmap, hBitmap);
	SelectObject(hdcTitles, hBitTitle);
	SelectObject(hdcCont, hBitCont);
	SelectObject(hdcScore, hBitScore);

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(hwnd, &mousePos);
	bool moused = false;


	for (int i = 0; i < 3; i++)
			{
				BitBlt(hdcBack, 15+i*20, 50, 20, 20, hdcBitmap, 0, 0, SRCCOPY);
				BitBlt(hdcBack, 95+i*20, 50, 20, 20, hdcBitmap, 20, 0, SRCCOPY);
				BitBlt(hdcBack, 95+i*20, 90, 20, 20, hdcBitmap, 20, 0, SRCCOPY);
				BitBlt(hdcBack, 95+i*20, 130, 20, 20, hdcBitmap, 20, 0, SRCCOPY);
				BitBlt(hdcBack, 175+i*20, 50, 20, 20, hdcBitmap, 40, 0, SRCCOPY);
				BitBlt(hdcBack, 255+i*20, 50, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
				BitBlt(hdcBack, 335+i*20, 50, 20, 20, hdcBitmap, 80, 0, SRCCOPY);
				BitBlt(hdcBack, 335+i*20, 130, 20, 20, hdcBitmap, 80, 0, SRCCOPY);
				BitBlt(hdcBack, 355, 70+i*20, 20, 20, hdcBitmap, 80, 0, SRCCOPY);
				BitBlt(hdcBack, 415+i*20, 50, 20, 20, hdcBitmap, 100, 0, SRCCOPY);
				BitBlt(hdcBack, 415+i*20, 90, 20, 20, hdcBitmap, 100, 0, SRCCOPY);
				BitBlt(hdcBack, 415+i*20, 130, 20, 20, hdcBitmap, 100, 0, SRCCOPY);
			}
			for (int i = 0; i < 4; i++)
			{
				BitBlt(hdcBack, 35, 70+i*20, 20, 20, hdcBitmap, 0, 0, SRCCOPY);
				BitBlt(hdcBack, 195, 70+i*20, 20, 20, hdcBitmap, 40, 0, SRCCOPY);
				BitBlt(hdcBack, 255, 70+i*20, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
			}
			BitBlt(hdcBack, 95, 70, 20, 20, hdcBitmap, 20, 0, SRCCOPY);
			BitBlt(hdcBack, 95, 110, 20, 20, hdcBitmap, 20, 0, SRCCOPY);
			BitBlt(hdcBack, 275, 90, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
			BitBlt(hdcBack, 295, 70, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
			BitBlt(hdcBack, 295, 110, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
			BitBlt(hdcBack, 295, 130, 20, 20, hdcBitmap, 60, 0, SRCCOPY);
			BitBlt(hdcBack, 415, 70, 20, 20, hdcBitmap, 100, 0, SRCCOPY);
			BitBlt(hdcBack, 455, 110, 20, 20, hdcBitmap, 100, 0, SRCCOPY);

	switch (menuState)
	{
	case 0:
		{			
			BitBlt(hdcBack, 150, 250, 200, 135, hdcTitles, 0, 0, SRCCOPY);

			if (mousePos.x > 200 && mousePos.x < 300)
				if (mousePos.y > 250 && mousePos.y < 290)
				{
					moused = true;
					BitBlt(hdcBack, 160, 240, 40, 50, hdcTitles, 215, 14, SRCCOPY);
					BitBlt(hdcBack, 295, 240, 40, 50, hdcTitles, 265, 12, SRCCOPY);
					if (lastSound != 1)
					{
						PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
						lastSound = 1;
					}
				}
			if (mousePos.x > 165 && mousePos.x < 340)
				if (mousePos.y > 300 && mousePos.y < 335)
				{
					moused = true;
					BitBlt(hdcBack, 120, 290, 40, 50, hdcTitles, 215, 14, SRCCOPY);
					BitBlt(hdcBack, 345, 290, 40, 50, hdcTitles, 265, 12, SRCCOPY);
					if (lastSound != 2)
					{
						PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
						lastSound = 2;
					}
				}
			if (mousePos.x > 150 && mousePos.x < 355)
				if (mousePos.y > 350 && mousePos.y < 385)
				{
					moused = true;
					BitBlt(hdcBack, 110, 340, 40, 50, hdcTitles, 215, 14, SRCCOPY);
					BitBlt(hdcBack, 360, 340, 40, 50, hdcTitles, 265, 12, SRCCOPY);
					if (lastSound != 3)
					{
						PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
						lastSound = 3;
					}
				}
			break;
		}

	case 1:
		{
			BitBlt(hdcBack, 85, 200, 310, 240, hdcCont, 0, 0, SRCCOPY);
			if (mousePos.x > 205 && mousePos.x < 285)
				if (mousePos.y > 400 && mousePos.y < 435)
				{
					moused = true;
					BitBlt(hdcBack, 165, 395, 40, 50, hdcCont, 315, 14, SRCCOPY);
					BitBlt(hdcBack, 282, 396, 40, 50, hdcCont, 320, 78, SRCCOPY);
					if (lastSound != 4)
					{
						PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
						lastSound = 4;
					}
				}
		}
		break;

	case 2:
		{
			BitBlt(hdcBack, 105, 210, 270, 235, hdcScore, 0, 0, SRCCOPY);
			if (mousePos.x > 195 && mousePos.x < 270)
				if (mousePos.y > 410 && mousePos.y < 445)
				{
					moused = true;
					BitBlt(hdcBack, 152, 403, 40, 50, hdcScore, 270, 10, SRCCOPY);
					BitBlt(hdcBack, 270, 403, 40, 50, hdcScore, 283, 74, SRCCOPY);
					if (lastSound != 5)
					{
						PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
						lastSound = 5;
					}
				}

			for (int i = 0; i < 5; i++)
			{
				BitBlt(hdcBack, 120, 245 + i * 30, 18, 18, hdcScore, 20 * (i+1), 240, SRCCOPY);
				BitBlt(hdcBack, 140, 245 + i * 30, 18, 18, hdcScore, 200, 240, SRCCOPY);
			}

			for (int i = 0; i < 5; i++)
				DrawScores(hdcBack, hdcScore, topscores[i], i);
		}
		break;

	default: break;
	}

	if (!moused)
		lastSound = 0;
	DeleteDC(hdcBitmap);
	DeleteDC(hdcTitles);
	DeleteDC(hdcCont);
	DeleteDC(hdcScore);
	DeleteObject(hBitTitle);
	DeleteObject(hBitCont);
	DeleteObject(hBitScore);
	return;
}
void Board::DrawScores(HDC hdcBack, HDC hdcScore, int scoreIn, int index)
{
	int score = scoreIn;
	if (score >= 100000)
		score = 99999;
	int divisor = 10000;
	for (int i = 0; i < 5; i++)
	{
		BitBlt(hdcBack, 200 + i * 20, 245 + index * 30, 18, 18, hdcScore, (score / divisor) * 20, 240, SRCCOPY);
		score -= (score / divisor) * divisor;
		divisor = divisor / 10;
	}

	return;
}
bool Board::GameOver(UINT wDeviceID, MCI_GENERIC_PARMS mciGenParms)
{
	if (gameoverRow >= 0 && counter % 3 == 0)
	{
		for (int i = 0; i < HORZELEMS; i++)
		{
			filled[i][gameoverRow] = 0;
		}
		gameoverRow--;
	}

	if (counter == 21)
	{
		mciSendCommand(wDeviceID, MCI_STOP, NULL, (DWORD_PTR) &mciGenParms);
		PlaySound(MAKEINTRESOURCE(IDR_GAMEOVER), hInst, SND_RESOURCE | SND_ASYNC);
	}

	if (counter > 125)
		return true;

	counter++;
	return false;
}

bool Board::IsGameOver(POINT spawnPoint)
{
	if (filled[spawnPoint.x][spawnPoint.y] > -1)
	{
		PlaySound(MAKEINTRESOURCE(IDR_CRASH), hInst, SND_RESOURCE | SND_ASYNC);
		return true;
	}
	else
		return false;
}

bool Board::DetectClear()
{
	bool lineClear = false;
	counter = 0;

	for (int i = 0; i < VERTELEMS; i++)
	{
		for (int j = 0; j < HORZELEMS; j++)
		{
			if (filled[j][i] < 0)
				break;
			if (j == HORZELEMS - 1)
			{
				lineClear = true;
				clearRow[numLines] = i;
				numLines++;
			}
		}
	}
	if (numLines > 0)
	{
		switch (numLines)
		{
		case 1:
			points += 100;
			break;
		case 2:
			points += 250;
			break;
		case 3:
			points += 500;
			break;
		case 4:
			points += 800;
			break;
		default: break;
		}
		cleared += numLines;
		level = cleared / 10 + 1;
	}
	return lineClear;
}

bool Board::AnimateClear(HDC hdcBack, HDC hdcMain)
{
	HDC hdcBitmap = CreateCompatibleDC(hdcMain);
	SelectObject(hdcBitmap, hBitmap);
	Rectangle(hdcBack, outlineRect.left, outlineRect.top, outlineRect.right, outlineRect.bottom);
	HFONT oldFont = (HFONT) SelectObject(hdcBack, hFontMain);
	Rectangle(hdcBack, 5, 5, 130, 60);
	wstring out1 = L"Score";
	TextOut(hdcBack, 10, 10, out1.c_str(), out1.length());
	wstringstream ss1;
	ss1 << points;
	TextOut(hdcBack, 35, 35, ss1.str().c_str(), ss1.str().length());
	Rectangle(hdcBack, 5, 105, 130, 200);
	wstring out2 = L"Next";
	TextOut(hdcBack, 10, 110, out2.c_str(), out2.length());
	Rectangle(hdcBack, 5, 235, 130, 360);
	wstring out3 = L"Level";
	wstring out4 = L"Lines";
	TextOut(hdcBack, 10, 240, out3.c_str(), out3.length());
	TextOut(hdcBack, 10, 300, out4.c_str(), out4.length());
	wstringstream ss3, ss4;
	ss3 << level;
	ss4 << cleared;
	TextOut(hdcBack, 35, 265, ss3.str().c_str(), ss3.str().length());
	TextOut(hdcBack, 35, 325, ss4.str().c_str(), ss4.str().length());
	Rectangle (hdcBack, 360, 10, 490, 310);
	wstring out5 = L"Pieces Played";
	TextOut(hdcBack, 365, 15, out5.c_str(), out5.length());
	for (int i = 0; i < 7; i++)
	{
		BitBlt(hdcBack, 375, 40 + 35*i, BLOCKSIZE, BLOCKSIZE, hdcBitmap, i*20, 0, SRCCOPY);
		wstringstream ssStats;
		ssStats << L"-  " << stats[i];
		TextOut(hdcBack, 410, 40 + 35*i, ssStats.str().c_str(), ssStats.str().length());
	}
	wstring out6 = L"Total:";
	TextOut(hdcBack, 400, 285, out6.c_str(), out6.length());
	wstringstream ss6;
	int total = 0;
	for (int i = 0; i < 7; i++)
		total += stats[i];
	ss6 << total;
	TextOut(hdcBack, 455, 285, ss6.str().c_str(), ss6.str().length());

	SelectObject(hdcBack, oldFont);
	
	if (counter < 1)
		PlaySound(MAKEINTRESOURCE(IDR_CLEAR), hInst, SND_RESOURCE | SND_ASYNC);
	if (counter < 22)
	{
		for (int j = 0; j < VERTELEMS; j++)
		{
			if (j==clearRow[0] || j==clearRow[1] || j==clearRow[2] || j==clearRow[3])
			{
				if (counter % 3 == 0)
				{
					for (int i = 0; i < HORZELEMS; i++)
						BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+j*BLOCKSIZE, BLOCKSIZE, BLOCKSIZE, hdcBitmap, 140, 0, SRCCOPY);
				}
				else
					for (int i = 0; i < HORZELEMS; i++)
						if (filled[i][j] >= 0)
							BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+j*BLOCKSIZE, BLOCKSIZE, BLOCKSIZE, hdcBitmap, filled[i][j] * BLOCKSIZE, 0, SRCCOPY);
			}
			else
			{
				for (int i = 0; i < HORZELEMS; i++)
						if (filled[i][j] >= 0)
							BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+j*BLOCKSIZE, BLOCKSIZE, BLOCKSIZE, hdcBitmap, filled[i][j] * BLOCKSIZE, 0, SRCCOPY);
			}
		}
		DeleteDC(hdcBitmap);
		counter++;
		return true;
	}

	if (counter < 25)
	{
		for (int k = 0; k < numLines; k++)
		{
			for (int j = clearRow[k]; j > 0; j--)
				for (int i = 0; i < HORZELEMS; i++)
					filled[i][j] = filled[i][j-1];
			for (int i = 0; i < HORZELEMS; i++)
				filled[i][0] = -1;
		}
		counter = 100;
		DeleteDC(hdcBitmap);
		return true;
	}

	if (counter < 110)
	{
		for (int j = clearRow[numLines-1]; j > 0; j--)
		{
			int linesToFall = 0;
			for (int k = numLines-1; k > -1; k--)
			{
				if (j <= clearRow[k] + (numLines-1 - k))
					linesToFall++;
			}
			int offset = 2*linesToFall*(counter-100);
			for (int i = 0; i < HORZELEMS; i++)
				BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+(j-linesToFall)*BLOCKSIZE+offset, BLOCKSIZE, BLOCKSIZE, hdcBitmap, filled[i][j] * BLOCKSIZE, 0, SRCCOPY);
		}
		for (int j = clearRow[numLines-1]+1; j < VERTELEMS; j++)
			for (int i = 0; i < HORZELEMS; i++)
				if (filled[i][j] >= 0)
					BitBlt(hdcBack, boardRect.left+i*BLOCKSIZE, boardRect.top+j*BLOCKSIZE, BLOCKSIZE, BLOCKSIZE, hdcBitmap, filled[i][j] * BLOCKSIZE, 0, SRCCOPY);
		
		DeleteDC(hdcBitmap);
		counter++;
		return true;
	}

	DeleteDC(hdcBitmap);
	counter = 0;
	for (int i = 0; i < numLines; i++)
		clearRow[i] = -1;
	numLines = 0;
	PlaySound(MAKEINTRESOURCE(IDR_THUD), hInst, SND_RESOURCE | SND_ASYNC);
	return false;
}

int Board::GetLevel() { return level; }
void Board::SetMenuState(int stateIn) { menuState = stateIn; }
int Board::GetMenuState() { return menuState; }
void Board::LoadScores()
{
	const wstring filename = L"data.ini";
	wifstream fin(filename.c_str());
	size_t semi = 0;
	size_t fail = -1;
	if (fin.good())
	{
		wstring input;
		getline(fin, input);
		for (int i = 0; i < 5; i++)
		{
			semi = input.find(';');
			if (semi != fail)
			{
				wstring data = input.substr(0,semi);
				input.erase(0,semi+1);
				topscores[i] = StringToInt(data);
			}
		}
	}
	fin.close();
	return;
}
void Board::SaveScores()
{
	int place = 6;
	for (int i = 4; i > -1; i--)
	{
		if (points > topscores[i])
			place = i;
	}
	if (place < 5)
	{
		const wstring filename = L"data.ini";
		wofstream fout(filename.c_str());
		wstringstream ssOut;
		for (int i = 4; i > place; i--)
		{
			topscores[i] = topscores[i-1];
		}
		topscores[place] = points;
		for (int i = 0; i < 5; i++)
		{
			ssOut << topscores[i] << L';';
		}
		fout << ssOut.str().c_str();
		fout.close();
	}

	return;
}
int Board::StringToInt(wstring stringIN)
{
	int intValue = 0;
	double base = 10;
	for (unsigned int i = 1; i <= stringIN.length(); i++)
	{
		switch (stringIN.c_str()[stringIN.length() - i])
		{
		case '0': break; 
		case '1': intValue += (int)pow(base, (int)(i-1)); break;
		case '2': intValue += 2 * (int)pow(base, (int)(i-1)); break;
		case '3': intValue += 3 * (int)pow(base, (int)(i-1)); break;
		case '4': intValue += 4 * (int)pow(base, (int)(i-1)); break;
		case '5': intValue += 5 * (int)pow(base, (int)(i-1)); break;
		case '6': intValue += 6 * (int)pow(base, (int)(i-1)); break;
		case '7': intValue += 7 * (int)pow(base, (int)(i-1)); break;
		case '8': intValue += 8 * (int)pow(base, (int)(i-1)); break;
		case '9': intValue += 9 * (int)pow(base, (int)(i-1)); break;
		default: return -1;
		}
	}
	return intValue;
}