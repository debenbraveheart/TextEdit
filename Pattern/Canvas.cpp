#include "stdafx.h"
#include "Canvas.h"


Canvas::Canvas():
mStartX(0),
mStartY(0),
mWidth(0),
mHeight(0),
mBitmapPtr(nullptr)
{
}

Canvas::Canvas(int startX, int startY, int width, int height):
mStartX(startX),
mStartY(startY),
mWidth(width),
mHeight(height)
{
	mBitmapPtr = GfxMgr::CreateBitmap(width,height);
}
Canvas::~Canvas()
{
}

void Canvas::OnResize(int startX, int startY, int width, int height)
{
	if (startX == mStartX && startY == mStartY)
	{
		if (width == mWidth && height == mHeight)
		{
			return;
		}
	}
	else
	{
		mStartX = startX;
		mStartY = startY;

		mWidth = width;
		mHeight = height;

		mBitmapPtr->OnResize(mWidth, mHeight);
	}
}

/*****
All Draw Commands ********************
******/
