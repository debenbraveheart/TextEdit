// Pattern.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Pattern.h"
#include "SystemMetrics.h"
#include "Controller.h"
#include "GfxMgr.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

Controller* gController;

shared_ptr<GfxMgr> gGfxPtr;

HWND hWnd;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	HACCEL hAccelTable;

	//Create the Controller[Singleton]
	gController = Controller::GetController();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PATTERN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	//SystemMetrics
	//CHARWIDTH = GetSystemMetrics(SM_CXSIZE);
	//CHARHEIGHT = GetSystemMetrics(SM_CYSIZE);


	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PATTERN));
	
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		/*bool gotMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (gotMessage)
		{
			if (msg.message != WM_QUIT)
			{
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
				break;
		}
		else
		{
			//RENDER
			//gController->Draw();
		}*/
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		
	}
	
	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PATTERN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PATTERN);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   //HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //initialize the graphics manager
   GfxMgr::init(hWnd);

   gController->SetHwnd(hWnd);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
#pragma comment(lib, "user32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_FILEOPEN:
			//open new files
			gController->OpenNewFile(string("C:\\Users\\dsahu\\Desktop\\xxx.txt"));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		/*
		char buf[1024];
		sprintf_s(buf, "%d   %d", CHARHEIGHT, APPHEIGHT);
		RECT rect = { 100, 100, 400, 400 };
		DrawTextA(hdc, buf, strnlen_s(buf, 1024), &rect, 1);
		rect.top = 100 + CHARHEIGHT;
		DrawTextA(hdc, buf, strnlen_s(buf, 1024), &rect, 1);
		*/

		/*
		HDC hdcBmp = CreateCompatibleDC(hdc);

		HBITMAP bmp = CreateCompatibleBitmap(hdc, APPWIDTH, APPHEIGHT);

		HBITMAP hbmold = static_cast<HBITMAP> (SelectObject(hdcBmp, bmp));

		RECT r = { 10, 10, 300, 300 };
		//FillRect(hdcBmp)
		Rectangle(hdcBmp, 0, 0, APPWIDTH, APPHEIGHT);
		HBRUSH brush = CreateSolidBrush(RGB(200,35.5,157));
		SelectObject(hdcBmp, brush);
		Rectangle(hdcBmp, 10, 10, 500, 500);
		
		HPEN pen = CreatePen(1, 3, RGB(100, 100, 100));
		SelectObject(hdcBmp, pen);
		char buf[1024] = "ssssssssssssss";
		r = { 200, 200, 400, 400 };
		DrawTextA(hdcBmp, buf, strnlen_s(buf, 1024), &r, 1);
		BitBlt(hdc, 0, 0, APPWIDTH, APPHEIGHT, hdcBmp, 0, 0, SRCCOPY);
		SelectObject(hdcBmp, hbmold);

		DeleteDC(hdcBmp);
		*/
		//
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_SIZE:
	{
		APPHEIGHT = HIWORD(lParam);
		APPWIDTH = LOWORD(lParam);

		gController->OnResize(APPWIDTH, APPHEIGHT);
		gController->Draw();
		break;
	}

	case WM_KEYUP:
	{
		//gController->OnKeyUp(wParam);
		//gController->Draw();
		break;
	}
	case WM_KEYDOWN:
	{
		gController->OnKeyDown(wParam);
		gController->Draw();
		break;
	}
	case WM_CHAR:
	{
		gController->OnCharKeyDown(wParam);
		gController->Draw();
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
