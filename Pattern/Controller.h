#pragma once
#include <iostream>
#include <vector>

#include "stdafx.h"
#include "Debug.h"

#include "PageView.h"

using namespace std;


class Controller
{
private:

	HWND mHwnd;
	Rect mClientBound;

	vector<unique_ptr<PageView>> *mPageViews;

	Rect mPageViewBound;

	Controller();

public:
	~Controller();

	static Controller* GetController();

	void OnResize(int width, int height);
	/*void OnKeyDown();
	void OnKeyUp();
	void OnMouseDown();
	void OnMouseUp();
*/
	void OpenNewFile(string file);
	
	void SetHwnd(HWND h){ mHwnd = h; }

	void OnKeyDown(unsigned int);
	void OnCharKeyDown(unsigned int);

	void Draw();

	Key GetKeyCode(unsigned int key);
};

