#include "stdafx.h"
#include "GfxMgr.h"


GfxMgr::GfxMgr()
{
}


GfxMgr::~GfxMgr()
{
}


#if WIN_ENV

HWND GfxMgr::mHwnd;

void GfxMgr::init(HWND hwnd)
{
	mHwnd = hwnd;

	/****** DO OTHER PROCESSING *******/
}

shared_ptr<Bitmap> GfxMgr::CreateBitmap(int width, int height)
{
	return make_shared<WinBitmap>(mHwnd, width, height);
}

#endif

shared_ptr<GfxMgr> GfxMgr::GetGfxPtr()
{
	return nullptr;
}