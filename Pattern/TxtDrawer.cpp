#include "stdafx.h"
#include "TxtDrawer.h"

TxtDrawer::TxtDrawer(string file) :
mFilePath(file)
{
	mLexer = new TxtLexer();
}


TxtDrawer::~TxtDrawer()
{
}

extern HWND hWnd;
void TxtDrawer::Draw()
{

#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nTxtDrawer::Draw\n");
	DEBUGOUT(buf);
#endif

	/*
		how to draw a txt file

		may be we can get the file line by line.
		
	*/
	
	FILE* file;
	fopen_s(&file,mFilePath.c_str(), "r");


	Rect r = { 0, 0, 0, 0 };
	//FillRect(hdcBmp)
	char buf1[4096];
	//r = { 100, 100, 1000, 500 };

	int rr = 0;

	rr=fread(buf1, 1, 4095, file);
	buf1[rr] = 0;
#ifdef DEBUG
	char buf2[8096];
	sprintf_s(buf2, "\nTxtDrawer::Draw %s\n",buf1);
	DEBUGOUT(buf2);
#endif

	SetMapMode(GetDC(NULL), MM_TEXT);
	TEXTMETRIC tm;
	GetTextMetrics(GetDC(hWnd), &tm);

	int fHeight = tm.tmHeight + tm.tmExternalLeading;

	int fWidth = tm.tmAveCharWidth;

	int k = 0;
	int l = 0;

	for (int i = 0; i < rr; i++)
	{

		char c = buf1[i];
		r.left = k*fWidth;
		r.right = r.left + fWidth;
		r.top = l*fHeight;
		r.bottom = r.top + fHeight;
		mCanvas->GetBitmap()->DrawChar(c, r);

		k ++;

		if ((k*fWidth) >= mBoundingRect.right - mBoundingRect.left)
		{
			k = 0;
			l++;
		}
		
	//		//TextOutA(hdcBmp, 0,0, buf1, strnlen_s(buf1,4096));//buf1, strnlen_s(buf1, 1024), &r, 1);
	}
	//mCanvas->GetBitmap()
	//mCanvas->GetBitmap()->DrawChar('c', r);
	//BitBlt(hdc, 0, 0, 1000, 500, hdcBmp, 0,0, SRCCOPY);
	
	mCanvas->GetBitmap()->Blit(mBoundingRect.left, mBoundingRect.top, mBoundingRect.right - mBoundingRect.left, mBoundingRect.bottom - mBoundingRect.top);
	

}

void TxtDrawer::SetCanvas(Canvas *canvas)
{

}

void TxtDrawer::SetBoundingRect(Rect r)
{
	mBoundingRect = r;

	mCanvas = make_shared<Canvas>(r.left, r.top, r.right - r.left, r.bottom - r.top);
}

void TxtDrawer::OnResize(Rect r)
{
	//DO
}