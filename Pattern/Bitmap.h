#pragma once
#include "stdafx.h"

class Bitmap
{
public:
	//Bitmap();
	//~Bitmap();

	/*
	what a bitmap does????

	this bitmap should be a device dependent bitmap
	*/

	virtual void SetWidth(int w) = 0;
	virtual void SetHeight(int h) = 0;

	virtual HDC GetHDC() = 0;

	virtual void OnResize(int w, int h) = 0;
	virtual void DrawChar(char c, Rect rect) = 0;
	virtual void DrawText(char *string, int strLen, Rect rect) = 0;
	virtual int GetWidthOfTheString(char *string, int count) = 0;

	virtual void ClearRect(Rect rect) = 0;
	virtual void Blit(int startX, int startY, int width, int height) = 0;

	virtual void CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi) = 0;
	virtual PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp) = 0;

	virtual void CreateCaret(int width, int height) = 0;
	virtual void ReleaseCraet() = 0;
	virtual void ShowCaret(int x, int y) = 0;
};

