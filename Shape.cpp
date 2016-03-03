#include "Shape.h"

Shape::Shape(void):
type(0),
pConfig(0),
currConfig(0)
{
	pt = new POINT[4];
	pLoc = pt;
	pConfig = new ConfigSet[4];
	pCLoc = pConfig;
	blockSize = BLOCKSIZE;

	for (int i = 0; i < 4; i++)
	{
		pLoc->x = 0;
		pLoc->y = 0;
		pLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 0;
		pCLoc++;
	}
}

Shape::Shape(HINSTANCE hInst, int typeIn, POINT pointIn):
type(typeIn),
pConfig(0),
currConfig(0)
{
	hBitmap = (HBITMAP)LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0,0,0);
	blockSize = BLOCKSIZE;
	pt = new POINT[4];
	pConfig = new ConfigSet[4];
	pCLoc = pConfig;
	pLoc = pt;

	switch (type)	       //configurations setup
	{
	case 0:
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 2;
		pCLoc->p3.y = 0;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 2;
		pCLoc++;
		pCLoc->p1.x = -2;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 0;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -2;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = -1;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 1;		
		break;
	case 1:
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 0;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = -1;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 0;
		break;
	case 2:
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = -1;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = 1;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 0;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = -1;
		pCLoc->p3.y = 1;
		break;
	case 3:
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = 1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = -1;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 1;
		break;
	case 4:
		for (int i = 0; i < 4; i++)
		{
			pCLoc->p1.x = 0;
			pCLoc->p1.y = -1;
			pCLoc->p2.x = 1;
			pCLoc->p2.y = 0;
			pCLoc->p3.x = 1;
			pCLoc->p3.y = -1;
			pCLoc++;
		}
		break;
	case 5:
		pCLoc->p1.x = 0;
		pCLoc->p1.y = -1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = -1;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = 1;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = 1;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = 0;
		break;
	case 6:
		pCLoc->p1.x = 0;
		pCLoc->p1.y = 1;
		pCLoc->p2.x = 0;
		pCLoc->p2.y = -1;
		pCLoc->p3.x = 1;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = 1;
		pCLoc->p1.y = 1;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = -1;
		pCLoc->p3.y = 0;
		pCLoc++;
		pCLoc->p1.x = 0;
		pCLoc->p1.y = 1;
		pCLoc->p2.x = -1;
		pCLoc->p2.y = 1;
		pCLoc->p3.x = 0;
		pCLoc->p3.y = -1;
		pCLoc++;
		pCLoc->p1.x = -1;
		pCLoc->p1.y = 0;
		pCLoc->p2.x = 1;
		pCLoc->p2.y = 0;
		pCLoc->p3.x = -1;
		pCLoc->p3.y = -1;
		break;
	default: break;
	}

	pCLoc = pConfig;
	pLoc->x = pointIn.x;   //pt[0]
	pLoc->y = pointIn.y;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p1.x;	//pt[1]
	pLoc->y = pt->y + pCLoc->p1.y * blockSize;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p2.x;	//pt[2]
	pLoc->y = pt->y + pCLoc->p2.y * blockSize;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p3.x;	//pt[3]
	pLoc->y = pt->y + pCLoc->p3.y * blockSize;
	pLoc = pt;
}

Shape::~Shape(void)
{
	delete[] pt;
	delete[] pConfig;
	pt = 0;
	pConfig = 0;
	pLoc = 0;
	pCLoc = 0;
	DeleteObject(hBitmap);
}

int Shape::GetType() { return type; }

void Shape::SetPoint(POINT pointIn) { pt[0] = pointIn; return;}

POINT* Shape::GetPoints() { return pt; }

ConfigSet* Shape::GetConfig(int& config) 
{ 
	config = currConfig;
	return pConfig; 
}

void Shape::Translate(int tDirectionIn, int amountIn)
{
	pLoc = pt;
	if (tDirectionIn == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			pLoc->y += amountIn;
			pLoc++;
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			pLoc->x += tDirectionIn;
			pLoc++;
		}
	}
	return;
}

void Shape::Rotate(int rDirectionIn)
{
	pCLoc = pConfig;
	
	currConfig += rDirectionIn;
	if (currConfig == 4)
		currConfig = 0;
	if (currConfig == -1)
		currConfig = 3;
	
	for (int i = 0; i < currConfig; i++)
		pCLoc++;

	pLoc = pt;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p1.x;	//pt[1]
	pLoc->y = pt->y + pCLoc->p1.y * blockSize;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p2.x;	//pt[2]
	pLoc->y = pt->y + pCLoc->p2.y * blockSize;
	pLoc++;
	pLoc->x = pt->x + pCLoc->p3.x;	//pt[3]
	pLoc->y = pt->y + pCLoc->p3.y * blockSize;
	pLoc = pt;
	return;
}

void Shape::Print(HDC hdcBack, HDC hdcMain, RECT rcIn)
{
	HDC hdcBitmap = CreateCompatibleDC(hdcMain);
	SelectObject(hdcBitmap, hBitmap);
	pLoc = pt;

	for (int i = 0; i < 4; i++)
	{
		if (pLoc->y > -1)
			BitBlt(hdcBack, rcIn.left + blockSize * pLoc->x, rcIn.top + pLoc->y, blockSize, blockSize, hdcBitmap, type * blockSize, 0, SRCCOPY);
		pLoc++;
	}

	DeleteDC(hdcBitmap);
	return;
}

void Shape::Print(HDC hdcBack, HDC hdcMain)
{
	HDC hdcBitmap = CreateCompatibleDC(hdcMain);
	SelectObject(hdcBitmap, hBitmap);
	pLoc = pt;
	pLoc++;

	BitBlt(hdcBack, pt->x, pt->y, blockSize, blockSize, hdcBitmap, type * blockSize, 0, SRCCOPY);

	for (int i = 0; i < 3; i++)
	{
		BitBlt(hdcBack, pt->x + blockSize*(pLoc->x - pt->x), pLoc->y, blockSize, blockSize, hdcBitmap, type * blockSize, 0, SRCCOPY);
		pLoc++;
	}

	DeleteDC(hdcBitmap);
	return;

}
