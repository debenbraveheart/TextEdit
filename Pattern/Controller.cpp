#include "stdafx.h"
#include "Controller.h"

Controller::Controller()
{
	mClientBound = { 0, 0, 0, 0 };
	mPageViewBound = { 0, 0, 0, 0 };

	mPageViews = new vector<unique_ptr<PageView>>() ;
}


Controller::~Controller()
{
}

static Controller* mController;

Controller* Controller::GetController()
{
	if (mController)
		return mController;
	else
	{
		mController = new Controller();
		return mController;
	}
}


void Controller::OnResize(int width, int height)
{
#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf,"\nController::OnReSize width:%d  height:%d\n", width, height);
	DEBUGOUT(buf);
#endif
	//Client bound
	mClientBound = { 0, 0, width, height };

	//change the PageView Bound
	mPageViewBound = mClientBound;

	/*
	 *	propagate the size changes to all the pageviews
	 */
	if (mPageViews->size() > 0){

		/*   smart pointer has a get method to get the raw pointer */
		PageView* pgView = mPageViews->at(0).get();
		
		pgView->SetBounds(mPageViewBound);

	}
}

void Controller::OnKeyDown(unsigned int key)
{
	if (mPageViews->size() > 0){

		/*   smart pointer has a get method to get the raw pointer */
		PageView* pgView = mPageViews->at(0).get();

		Key k = GetKeyCode(key);
		if (k != Key::KEY_INVALID )
			pgView->OnKeyDown(k);

	}
}

void Controller::OnCharKeyDown(unsigned int key)
{
	if (mPageViews->size() > 0){

		/*   smart pointer has a get method to get the raw pointer */
		PageView* pgView = mPageViews->at(0).get();

		pgView->OnCharKeyDown(key);

	}
}
void Controller::Draw()
{
	if (mPageViews->size() > 0){

		/*   smart pointer has a get method to get the raw pointer */
		PageView* pgView = mPageViews->at(0).get();

		pgView->Draw();

	}
}

void Controller::OpenNewFile(string file)
{
	DEBUGOUT(file.c_str());

	//create a new pageview
	
	unique_ptr<PageView> pgView( new PageView());

	PageView *pgViewRaw = pgView.get();

	mPageViews->emplace_back(move(pgView));

	//set the bounding rect of the pageview
	
	pgViewRaw->SetBounds(mPageViewBound);

	//set the Document
	Document* doc = new Document(file);
	pgViewRaw->SetDocument(doc);

	pgViewRaw->Draw();
	

}

Key Controller::GetKeyCode(unsigned int key)
{
	switch (key)
	{
		case VK_DOWN:
			return Key::KEY_DOWN;
		case VK_UP:
			return Key::KEY_UP;
		case VK_RIGHT:
			return Key::KEY_RIGHT;
		case VK_LEFT:
			return Key::KEY_LEFT;
		case VK_RETURN:
			return Key::KEY_RETURN;
		case VK_END:
			return Key::KEY_END;
		case VK_HOME:
			return Key::KEY_HOME;
		//TODO all other keys
	}
	return Key::KEY_INVALID;
}