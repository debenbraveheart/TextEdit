#include "stdafx.h"
#include "Blit.h"

Blitter::Blitter()
{
}


Blitter::~Blitter()
{
}

void Blitter::SetBitmap(BITMAP* bmp, int srcStartX, int srcStartY, int destStartX, int destStartY, int width, int height)
{
	bitmap = bmp;
	srcStartX = srcStartX;
	srcStartY = srcStartY;
	destStartX = destStartX;
	destStartY =destStartY;
	width  = width;
	height = height;
}

void Blitter::Blit()
{
//	HDC hdc = GetDC(hWnd);
//	HDC hdcBmp = CreateCompatibleDC(hdc);
//
//	HBITMAP bmp = CreateCompatibleBitmap(hdc, 1000, 500);
//
//	HBITMAP hbmold = static_cast<HBITMAP> (SelectObject(hdcBmp, bmp));
//
//	RECT r = { 0, 0, 300, 300 };
//	//FillRect(hdcBmp)
//	Rectangle(hdcBmp, 0, 0, 1000, 500);
//	HBRUSH brush = CreateSolidBrush(RGB(200, 35.5, 157));
//	SelectObject(hdcBmp, brush);
//	Rectangle(hdcBmp, 10, 10, 500, 500);
//
//	HPEN pen = CreatePen(1, 3, RGB(100, 100, 100));
//	SelectObject(hdcBmp, pen);
//	char buf1[4096];
//	r = { 100, 100, 1000, 500 };
//
//	int rr = 0;
//
//	rr = fread(buf1, 1, 4095, file);
//	buf1[rr] = 0;
//#ifdef DEBUG
//	char buf2[8096];
//	sprintf_s(buf2, "\nTxtDrawer::Draw %s\n", buf1);
//	DEBUGOUT(buf2);
//#endif
//
//	TextOutA(hdcBmp, 0, 0, buf1, strnlen_s(buf1, 4096));//buf1, strnlen_s(buf1, 1024), &r, 1);
//
//	BitBlt(hdc, 0, 0, 1000, 500, hdcBmp, 0, 0, SRCCOPY);
//	SelectObject(hdcBmp, hbmold);
//
//	DeleteDC(hdcBmp);
}