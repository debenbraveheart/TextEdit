#pragma once

#include "stdafx.h"
#include "DocDrawer.h"
#include "TxtLexer.h"
#include "Canvas.h"
#include "LineBuffer.h"

class TxtDrawer : public DocDrawer
{
private:
	TxtLexer* mLexer;
	string mFilePath;

	shared_ptr<Canvas> mCanvas;

	Rect mBoundingRect;

	shared_ptr<LineBuffer> mLineBuffer;


	/*all the data about the view*/
	int mCurrentStart;
	int mCurrentEnd;
	int mTotalLines;
	int mTotalVisibleLines;
	int mTotalVisbileChars;
	int mOldCurrentStart;

	/*
	 *   mCuurentLine = mCurrentStart + mCaretLine
	 */
	int mCurrentLine;
	int mCurrentOffset; //the caret offset
 
	int mCaretLine;
	int mCaretOffset;

	bool mStateChanged;

	int ComputeTotalVisibleLines(int screenHeight, int fontWidth);
	int ComputeTotalCharsInALine(int screenWidth, int fontWidth);

	int ComputeFontHeight();
	int ComputeFontWidth();

	void ChangeCaretToNewLine(int lineNo);
	void ChangeCaretToNewOffset(int offset);
	void UpdateCaret();

	void AddNewLine(int afterLineNo);
public:
	TxtDrawer(string file);
	~TxtDrawer();

	void Draw();

	void SetCanvas(Canvas*);

	void SetBoundingRect(Rect r);
	void OnResize(Rect r);
	void OnKeyDown(Key key);
	void OnCharKeyDown(unsigned int key);
};

