#pragma once

#include "stdafx.h"
#include "Line.h"

class LineBuffer
{
private:
	vector<shared_ptr<Line> > mLinesBuff; //buffer for multiple lines
	
	int mTotalLines;  //total lines in the Buffer
	int mStartLine;
	int mEndLine;
public:
	LineBuffer();
	LineBuffer(int noOfLines, int startLine, int endLine);
	~LineBuffer();


	void InsertLine(char *string, int count);
	void InsertLineAfter(int lineNo, char *string, int count);
	int InsertData(char *string, int count);

	int GetLine(int lineNo, int startOffset, int count, char *dest);

	//return no of virtual char (means how many displayable char)
	//may be returning actual char
	int GetLineCount(int lineNo);

	int InsertChar(int lineNo, char c, unsigned int index);
	int DeleteChar(int lineNo, unsigned int index);
	int InsertWord(char *word, unsigned int index);
	int DeleteWord(unsigned int index);

	int DeleteLine(unsigned int line);

	int GetTotalLines(){ return mTotalLines; }
};

