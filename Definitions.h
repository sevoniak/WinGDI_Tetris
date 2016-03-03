#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <Windows.h>
#include <sstream>
#include <string>
#include <mmsystem.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <math.h>
#include "resource.h"
using std::wstringstream;
using std::wstring;
using std::wifstream;
using std::wofstream;


#define BLOCKSIZE 20
#define HORZELEMS 10
#define VERTELEMS 20
#define BOARDWIDTH HORZELEMS*BLOCKSIZE
#define BOARDHEIGHT VERTELEMS*BLOCKSIZE
#define GAMESPEED 50
#define STEPS 2