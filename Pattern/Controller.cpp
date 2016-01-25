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