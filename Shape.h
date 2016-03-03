#pragma once
#include "Definitions.h"

struct ConfigSet
{
	POINT p1;
	POINT p2;
	POINT p3;
};

class Shape
{
	int type;
	int currConfig;
	int blockSize;
	POINT* pt;
	POINT* pLoc;
	ConfigSet* pConfig;
	ConfigSet* pCLoc;
	HBITMAP hBitmap;
	
public:
	Shape(void);
	Shape(HINSTANCE hInst, int typeIn, POINT pointIn);
	~Shape(void);

	int GetType();
	void SetPoint(POINT pointIn);
	POINT* GetPoints();
	ConfigSet* GetConfig(int& config);
	void Translate(int tDirectionIn, int amount);
	void Rotate(int rDirectionIn);
	void Print(HDC hdcBack, HDC hdcMain, RECT rcIn);
	void Print(HDC hdcBack, HDC hdcMain);
};
