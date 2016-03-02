#pragma once
#include "stdafx.h"
#include "Bitmap.h"
#include "Rect.h"

class WinBitmap : public Bitmap
{

private:
	HWND mHwnd;
	HDC mHdcBitmap;

	HBITMAP mBitmapOld;

	HBITMAP mBitmap;

	int mWidth;
	int mHeight;


public:
	WinBitmap(HWND hwnd);
	WinBitmap(HWND hwnd, int w, int h);
	~WinBitmap();

	HDC GetHDC();
	/*
	what a bitmap does????

	this bitmap should be a device dependent bitmap
	*/

	void SetWidth(int w){ mWidth = w; }
	void SetHeight(int h){ mHeight = h; }

	void OnResize(int w, int h);

	void DrawText(char *string, int strLen, Rect rect);
	int GetWidthOfTheString(char *string, int count);

	void DrawChar(char c, Rect rect);
	void ClearRect(Rect rect);

	void Blit(int startX, int startY, int width, int height);

	void CreateCaret(int width, int height);
	void ReleaseCraet();
	void ShowCaret(int x, int y);

	void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi);
	PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
};

