#pragma once

#include "stdafx.h"
#include "GfxMgr.h"
#include "Bitmap.h"

/*
*  canvas is an abstarction of drawing elements. 
*  
* Blits should be done from here.
		It should have the authority to create any mechanism (like bitmaps) which could then
		be used to blit(or use the drawing commands)


* it should support all the graphics operations
* it also should support fonts and different colors

*	bounds
*/
class Canvas
{
private:
	int mStartX;
	int mStartY;
	int mWidth;
	int mHeight;

	shared_ptr<Bitmap> mBitmapPtr;

public:
	Canvas();
	Canvas(int startX, int startY, int width, int height);
	~Canvas();

	void OnResize(int startX, int startY, int width, int height);

	/*****
		All Draw Commands ********************
	******/

	shared_ptr<Bitmap> GetBitmap(){ return mBitmapPtr; }
};

