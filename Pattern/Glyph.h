#pragma once

#include "Rect.h"

class Window;

class Point;

class Glyph
{
private:
	Rect mRect;
public:
	Glyph();
	~Glyph();

	virtual void Draw(Window*) = 0;

	virtual void SetBounds(Rect&) = 0;

	virtual bool Intersects(const Point&) = 0;

	virtual void Insert(Glyph*, int) = 0;

	virtual void Remove(Glyph*) = 0;
	
	virtual Glyph* Child(int) = 0;

	virtual Glyph* Parent() = 0;
};

