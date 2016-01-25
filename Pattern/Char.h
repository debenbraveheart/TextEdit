#pragma once

#include "Glyph.h"

class Char	:	public Glyph
{
public:
	Char();
	~Char();

	void Draw(Window*);

	void Bounds(Rect&);

	bool Intersects(const Point&);

	void Insert(Glyph*, int);

	void Remove(Glyph*);

	Glyph* Child(int);

	Glyph* Parent();

};

