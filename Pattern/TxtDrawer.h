#pragma once

#include "stdafx.h"
#include "DocDrawer.h"
#include "TxtLexer.h"
#include "Canvas.h"

class TxtDrawer : public DocDrawer
{
private:
	TxtLexer* mLexer;
	string mFilePath;

	shared_ptr<Canvas> mCanvas;

	Rect mBoundingRect;

public:
	TxtDrawer(string file);
	~TxtDrawer();

	void Draw();

	void SetCanvas(Canvas*);

	void SetBoundingRect(Rect r);
	void OnResize(Rect r);
};

