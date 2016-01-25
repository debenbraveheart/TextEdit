#pragma once

#include "Glyph.h"
#include "Document.h"
#include "Canvas.h"

class PageView : public Glyph
{
public:
	PageView();
	~PageView();

	void Draw(Window*);
	void Draw();

	void SetBounds(Rect&);

	bool Intersects(const Point&);

	void Insert(Glyph*, int);

	void Remove(Glyph*);

	Glyph* Child(int);

	Glyph* Parent();

	void SetDocument(Document*);
private:
	Rect mBoundingRect;
	Document* mDocumentPtr;
	DocDrawer* mDocDrawerPtr;

	shared_ptr<Canvas> mCanvas;

	
};
