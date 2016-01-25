#pragma once

#include "stdafx.h"
#include "Canvas.h"

class DocDrawer
{
public:
	DocDrawer();
	~DocDrawer();

	virtual void Draw() = 0;
	virtual void SetCanvas(Canvas*) = 0;

	virtual void OnResize(Rect r) = 0;

	virtual void SetBoundingRect(Rect r) = 0;
};

