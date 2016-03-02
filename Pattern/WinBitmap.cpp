#include "stdafx.h"
#include "WinBitmap.h"


WinBitmap::WinBitmap(HWND hwnd) :
mHwnd(hwnd),
mWidth(0),
mHeight(0),
mHdcBitmap(nullptr),
mBitmapOld(nullptr),
mBitmap(nullptr)
{
}

WinBitmap::WinBitmap(HWND hwnd, int w, int h) :
mHwnd(hwnd),
mWidth(w),
mHeight(h),
mBitmapOld(nullptr)
{
	HDC hdc = GetDC(mHwnd);

	mHdcBitmap = CreateCompatibleDC(hdc);

	mBitmap = CreateCompatibleBitmap(hdc, mWidth, mHeight);

	DeleteDC(hdc);
}

WinBitmap::~WinBitmap()
{
	DeleteDC(mHdcBitmap);
}

HDC WinBitmap::GetHDC()
{
	return mHdcBitmap;
}
void WinBitmap::OnResize(int w, int h)
{
	if (mWidth == w && mHeight == h)
	{
		return;
	}

	mWidth = w;
	mHeight = h;

	//resize the bitmap

	HDC hdc = GetDC(mHwnd);

	mBitmap = CreateCompatibleBitmap(hdc, mWidth, mHeight);

	DeleteDC(hdc);
}


void WinBitmap::DrawChar(char c, Rect rect)
{
	char buf[2];
	buf[0] = c;
	buf[1] = 0;

	RECT r;
	r.bottom  = rect.bottom;
	r.top = rect.top;
	r.left = rect.left;
	r.right = rect.right;

	mBitmapOld = static_cast<HBITMAP> (SelectObject(mHdcBitmap, mBitmap));

	//HBRUSH brush = CreateSolidBrush(RGB(200, 35.5, 157));
	//SelectObject(mHdcBitmap, brush);

	//HPEN pen = CreatePen(1, 3, RGB(100, 100, 100));
	//SelectObject(mHdcBitmap, pen);

	//TextOutA(mHdcBitmap, buf, 2, &r, DT_CENTER, NULL);
	
	//intercharspacing
	//SetTextCharacterExtra(mHdcBitmap, 6);
	//SetBkMode(mHdcBitmap, TRANSPARENT);
	//setBackground color
	SetBkColor(mHdcBitmap, RGB(230, 0, 0));
	//SetTextColor(mHdcBitmap, RGB(230, 0, 0));

	DrawTextA(mHdcBitmap, buf, 2, &r, DT_SINGLELINE | DT_NOCLIP);
	
	//BitBlt(GetDC(mHwnd), 0, 0, 1000, 500, mHdcBitmap, 0, 0, SRCCOPY);

	SelectObject(mHdcBitmap, mBitmapOld);

	char buf1[200];
	sprintf_s(buf1, "\n%d\n", GetDeviceCaps(mHdcBitmap, LOGPIXELSY));
	OutputDebugStringA(buf1);

}

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib") 


void WinBitmap::DrawText(char *string, int strlen, Rect rect)
{
	RECT r;
	r.bottom = rect.bottom;
	r.top = rect.top;
	r.left = rect.left;
	r.right = rect.right;

	OutputDebugStringA(string);

	mBitmapOld = static_cast<HBITMAP> (SelectObject(mHdcBitmap, mBitmap));

	//HBRUSH brush = CreateSolidBrush(RGB(200, 35.5, 157));
	//SelectObject(mHdcBitmap, brush);

	//HPEN pen = CreatePen(1, 3, RGB(100, 100, 100));
	//SelectObject(mHdcBitmap, pen);

	//TextOutA(mHdcBitmap, buf, 2, &r, DT_CENTER, NULL);

	//intercharspacing
	//SetTextCharacterExtra(mHdcBitmap, 6);
	//SetBkMode(mHdcBitmap, TRANSPARENT);
	//setBackground color
	//SetBkColor(mHdcBitmap, RGB(230, 0, 0));
	SetTextColor(mHdcBitmap, RGB(230, 0, 0));

	/***************************************************
	**** can we use direct write here  
	*/
	DrawTextA(mHdcBitmap, string, strlen, &r, DT_SINGLELINE | DT_NOCLIP);
	
	/***************************/
	//BitBlt(GetDC(mHwnd), 0, 0, 1000, 500, mHdcBitmap, 0, 0, SRCCOPY);

	SelectObject(mHdcBitmap, mBitmapOld);


}

int WinBitmap::GetWidthOfTheString(char *string, int count)
{
	SIZE a;
	GetTextExtentPoint32A(mHdcBitmap, string, count, &a);

	return a.cx;
}
void WinBitmap::ClearRect(Rect rect)
{
	RECT r;
	r.left = rect.left;
	r.right = rect.right;
	r.top = rect.top;
	r.bottom = rect.bottom;

	mBitmapOld = static_cast<HBITMAP> (SelectObject(mHdcBitmap, mBitmap));

	HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
	//SelectObject(mHdcBitmap, hbr);
	//HBRUSH brush = CreateSolidBrush(RGB(200, 35.5, 157));
	//SelectObject(mHdcBitmap, brush);
	//Rectangle(mHdcBitmap, 10, 10, 500, 500);
	FillRect(mHdcBitmap, &r, hbr);
	SelectObject(mHdcBitmap, mBitmapOld);
}
void WinBitmap::Blit(int startX, int startY, int width, int height)
{
	//PBITMAPINFO pbi = CreateBitmapInfoStruct(mBitmap);
	//CreateBMPFile(L"C:\\Users\\dsahu\\Documents\\bitmap\\ddd.bmp", pbi);
	mBitmapOld = static_cast<HBITMAP> (SelectObject(mHdcBitmap, mBitmap));

	BitBlt(GetDC(mHwnd), startX, startY, width, height, mHdcBitmap, startX, startY, SRCCOPY);

	SelectObject(mHdcBitmap, mBitmapOld);

}

PBITMAPINFO WinBitmap::CreateBitmapInfoStruct(HBITMAP hBmp)
{
	HWND hwnd = mHwnd;
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// Retrieve the bitmap color format, width, and height.  
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		("GetObject", hwnd);

	// Convert the color format to a count of bits.  
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure  
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
	// data structures.)  

	if (cClrBits < 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER) +
		sizeof(RGBQUAD) * (1 << cClrBits));

	// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
		sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.  

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.  
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color  
	// indices and store the result in biSizeImage.  
	// The width must be DWORD aligned unless the bitmap is RLE 
	// compressed. 
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the  
	// device colors are important.  
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

void WinBitmap::CreateCaret(int width, int height)
{
	::CreateCaret(mHwnd, (HBITMAP)NULL, width, height);
	::SetCaretPos(100, 100);
	::ShowCaret(mHwnd);
}
void WinBitmap::ReleaseCraet()
{
	DestroyCaret();
}
void WinBitmap::ShowCaret(int x, int y)
{
	::SetCaretPos(x, y);
	::ShowCaret(mHwnd);
}

void WinBitmap::CreateBMPFile(LPTSTR pszFile, PBITMAPINFO pbi)
{

	HWND hwnd = mHwnd;
	HBITMAP hBMP = mBitmap;
	HDC hDC = GetWindowDC(hwnd);
	HANDLE hf;                 // file handle  
	BITMAPFILEHEADER hdr;       // bitmap file-header  
	PBITMAPINFOHEADER pbih;     // bitmap info-header  
	LPBYTE lpBits;              // memory pointer  
	DWORD dwTotal;              // total count of bytes  
	DWORD cb;                   // incremental count of bytes  
	BYTE *hp;                   // byte pointer  
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
		DEBUGOUT("GlobalAlloc");

	// Retrieve the color table (RGBQUAD array) and the bits  
	// (array of palette indices) from the DIB.  
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		DEBUGOUT("GetDIBits",);
	}

	// Create the .BMP file.  
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE)
		DEBUGOUT("CreateFile");
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
	// Compute the size of the entire file.  
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.  
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.  
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
		(LPDWORD)&dwTmp, NULL))
	{
		DEBUGOUT("WriteFile");
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		DEBUGOUT("WriteFile");

	// Copy the array of color indices into the .BMP file.  
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		DEBUGOUT("WriteFile");

	// Close the .BMP file.  
	if (!CloseHandle(hf))
		DEBUGOUT("CloseHandle");

	// Free memory.  
	GlobalFree((HGLOBAL)lpBits);
}