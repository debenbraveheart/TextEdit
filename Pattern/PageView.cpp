#include "stdafx.h"
#include "PageView.h"


PageView::PageView()
{
}


PageView::~PageView()
{
}

void PageView::Draw(Window*)
{

}
void PageView::Draw()
{
#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nPageView::Draw \n");
	DEBUGOUT(buf);
#endif

	mDocDrawerPtr->Draw();
}

void PageView::SetBounds(Rect& rect)
{
	mBoundingRect = { rect };

#if 0
	char buf[1024];
	sprintf_s(buf, "\nPageView::SetBounds left:%d  top:%d  right:%d  bottom:%d\n", mBoundingRect.left, mBoundingRect.top, mBoundingRect.right, mBoundingRect.bottom);
	DEBUGOUT(buf);
#endif
}

bool PageView::Intersects(const Point&)
{
	return NULL;
}

void PageView::Insert(Glyph*, int)
{

}

void PageView::Remove(Glyph*)
{

}

Glyph* PageView::Child(int)
{
	return NULL;
}

Glyph* PageView::Parent()
{
	return NULL;
}


void PageView::SetDocument(Document* doc)
{
	mDocumentPtr = doc;

	//create a document pointer according to the provided file
	mDocDrawerPtr = DocumentDrawerFactory::CreateDocDrawer(doc->GetFilePath());
	mDocDrawerPtr->SetBoundingRect(mBoundingRect);
}