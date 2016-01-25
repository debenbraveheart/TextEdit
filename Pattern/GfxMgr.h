#pragma once

#include "stdafx.h"
#include "Bitmap.h"
/*
	Job for a graphics manager

	1) Get a canvas/bitmap and create an id out of it and store it.
	
	2) Clients call its member functions
		To draw something onto it.
		
		sub properties can be defined (like fonts, color etc for a text)






*/
#define WIN_ENV 1


#if WIN_ENV
#include "WinBitmap.h"
#endif

class GfxMgr
{
private:
	shared_ptr<GfxMgr> mGfxMagrPtr;
	
#if WIN_ENV

	static HWND mHwnd;

#endif

	GfxMgr();
public:
	~GfxMgr();

#if WIN_ENV
	static void init(HWND hwnd);
	static shared_ptr<Bitmap> CreateBitmap(int width, int height);
#endif

	static shared_ptr<GfxMgr> GetGfxPtr();

};

