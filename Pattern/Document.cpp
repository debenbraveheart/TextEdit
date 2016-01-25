#include "stdafx.h"
#include "Document.h"


Document::Document()
{
}

Document::Document(string file):
mPath(file)
{
	Init();
}

Document::~Document()
{
}

void Document::SetFilePath(string path)
{
	mPath = path;
}
string Document::GetFilePath(){ return mPath; }

void Document::Init()
{
#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nDocument::Init filename:%s\n", mPath.c_str());
	DEBUGOUT(buf);
#endif


}

/*void Document::Draw()
{
#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nDocument::Draw\n", mPath.c_str());
	DEBUGOUT(buf);
#endif

	
	mDocDrawerPtr->Draw();

}*/