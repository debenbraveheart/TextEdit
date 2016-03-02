#include "stdafx.h"
#include "LineBuffer.h"


LineBuffer::LineBuffer() :
mTotalLines(0),
mStartLine(0),
mEndLine(0)
{

}

LineBuffer::LineBuffer(int noOfLines, int startLine, int endLine) :
mTotalLines(noOfLines),
mStartLine(0),
mEndLine(0)
{
	if (noOfLines == 0 || startLine < 0 || endLine < startLine + 1 || endLine >= noOfLines)
	{
		//TODO EXCEPTION
	}

}

LineBuffer::~LineBuffer()
{
}

void LineBuffer::InsertLine(char *string, int count)
{
	shared_ptr<Line> line;

	line = make_shared<Line>();

	//copy the chars in the line
	line->Copy(string, count);

	//push to the vector
	mLinesBuff.push_back(line);
}

void LineBuffer::InsertLineAfter(int lineNo, char *string, int count)
{
	shared_ptr<Line> line;

	line = make_shared<Line>();

	//copy the chars in the line
	line->Copy(string, count);

	//push to the vector at specified location
	std::vector<shared_ptr<Line>>::iterator it;

	it = mLinesBuff.begin();

	if (lineNo == -1)
		mLinesBuff.push_back(line);
	else
		mLinesBuff.insert(it + lineNo + 1, line);

	++mTotalLines;
}
int LineBuffer::InsertData(char *string, int count)
{
	for (int i = 0; i < count; i++)
	{
		int j = 0;

		int k = i;
		while (i < count)
		{
			if (string[i] == '\n')
			{
				break;		
			}
			j++;
			i++;
		}
		//string+k is the new start
		InsertLine(string + k, j);
		
		mTotalLines++;

	}

	return mTotalLines;
	//TODO take care of the case when remianing of the line is in the next Insertdata
}


int LineBuffer::GetLine(int lineNo, int startOffset, int count, char *dest)
{
	if (!dest)
		return 0;
	if (lineNo > mTotalLines-1 || lineNo < 0)
		return 0;
	shared_ptr<Line> line = mLinesBuff.at(lineNo);

	return line->GetLine(count, startOffset, dest);
}

int LineBuffer::GetLineCount(int lineNo)
{
	if (lineNo > mTotalLines-1)
		return 0;
	shared_ptr<Line> line = mLinesBuff.at(lineNo);
	
	return line->GetLineCount();
}

int LineBuffer::InsertChar(int lineNo, char c, unsigned int index)
{
	if (mTotalLines == 0)
	{
		//craete another line
		shared_ptr<Line> l = make_shared<Line>();
		

		//push to the vector at specified location
		std::vector<shared_ptr<Line>>::iterator it;

		it = mLinesBuff.begin();

		mLinesBuff.insert(it,l);
		
		mTotalLines++;

		return l->InsertChar(c, 0);;
	}
	if (lineNo > mTotalLines - 1)
		return 0;
	shared_ptr<Line> line = mLinesBuff.at(lineNo);

	return line->InsertChar(c, index);
}

int LineBuffer::DeleteChar(int lineNo, unsigned int index)
{
	if (lineNo > mTotalLines - 1)
		return 0;
	shared_ptr<Line> line = mLinesBuff.at(lineNo);

	return line->DeleteChar(index);
}

int LineBuffer::InsertWord(char *word, unsigned int index)
{
	//TODO	
	return 1;
}
int LineBuffer::DeleteWord(unsigned int index)
{
	//TODO
	return 1;
}

int LineBuffer::DeleteLine(unsigned int line)
{
	if (line >= mTotalLines)
		return 0;

	//push to the vector at specified location
	std::vector<shared_ptr<Line>>::iterator it;

	it = mLinesBuff.begin();

	mLinesBuff.erase(it + line);

	--mTotalLines;

	return 1;
}