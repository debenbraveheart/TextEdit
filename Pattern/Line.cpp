#include "Line.h"



Line::Line():
mNextLinePtr(nullptr)
{
	//mBuffer = (char*)malloc(sizeof(char)*(MAXBUFSIZE));
	mPieceDescriptor = nullptr;
	/*initilize the add buffer*/
	mAddBuffer = make_shared<AddBuffer>();

	mOriginalBuffer = nullptr;
	mNoOfChar = 0;
}


Line::~Line()
{
	if (mNextLinePtr)
		delete mNextLinePtr.get();

	if (mOriginalBuffer)
		free(mOriginalBuffer);
	mOriginalBuffer = nullptr;
	mVisibleBuffer = nullptr;

	mPieceDescriptor = nullptr;
	mInsertLastDescriptor = nullptr;
	mDeleteLastDescriptor = nullptr;
	mAddBuffer = nullptr;
}


void Line::Copy(char *buff, int count)
{
	/*
	if (count <= MAXBUFSIZE)
	{
		memcpy_s(mBuffer, MAXBUFSIZE, buff, count);
#if 0
		char c = buff[count];
		buff[count] = 0;
		OutputDebugStringA(buff);
		buff[count] = c;
		char buf2[1024];
		sprintf_s(buf2, "\n--------------%d -------------\n", count);
		OutputDebugStringA(buf2);
#endif

		//store the count
		mNoOfChar = count;
	}
	else //copy into next LinePtr (linkedList)
	{
		memcpy_s(mBuffer, MAXBUFSIZE, buff, MAXBUFSIZE);

		shared_ptr<Line> newLine = make_shared<Line>();
		mNextLinePtr = newLine;
		newLine->Copy(buff + MAXBUFSIZE, count - MAXBUFSIZE);

		mNoOfChar = MAXBUFSIZE;
	}
	*/
#if 0
	char c = buff[count];
	buff[count] = 0;
	cout<<buff;
	buff[count] = c;
	char buf2[1024];
	sprintf_s(buf2, "\n--------------%d -------------\n", count);
	cout<<buf2;
#endif
	if (count > 0)
	{

		mOriginalBuffer = (char*)malloc(sizeof(char)*(count));
		memcpy_s(mOriginalBuffer, MAXBUFSIZE, buff, count);
		
	}

	mNoOfChar = count;
	/* Initialize the Piece table [here the first destructor]*/
	mPieceDescriptor = make_shared<PieceDescriptor>();
	mPieceDescriptor->offset = 0;
	mPieceDescriptor->length = count;
	
}

int Line::GetLine(int count, int startOffset, char *buf)
{
	/*
	int total = 0;

	int charCount = count;
	
	if (count > MAXBUFSIZE)
	{
		charCount = MAXBUFSIZE;
		if (mNextLinePtr)
		{
			total += mNextLinePtr->GetLine(count - MAXBUFSIZE, buf + MAXBUFSIZE);
		}
	}
	
	total += memcpy_s(buf, charCount, mBuffer, charCount);

	if (mNoOfChar < count)
		return mNoOfChar;
	else
		return total;

#if 0
	OutputDebugStringA(buf);
	OutputDebugStringA("]]]]]]]]]]]]]]]]\n");
#endif
	*/

	/*
	* Here we give them the right line
	*/

	/*
	int charCount = count - startOffset;
	if (mNoOfChar - startOffset < count)
		charCount = mNoOfChar - startOffset;
	
	memcpy_s(buf, charCount, mOriginalBuffer, charCount);


	return charCount;
	*/
	if (!buf)
		return -1;

	//total offset is greater than the total no of chars present
	if (startOffset >= mNoOfChar)
		return 0;  //we might be in a line where no char is there(blank line)

	int offset = startOffset;

	PieceDescriptorP p = mPieceDescriptor.get();

	int totalCount = 0;

	while (p)
	{
		int len = p->length;

		if ((totalCount + p->length) > count)
			len = count - totalCount;
		
		if (offset > len)
		{
			offset -= len;
			p = p->next.get();
			continue;
		}
	
		len -= offset;
			//copy the length buffer to the dest
		if (p->addBuffer)
		{
			mAddBuffer->CopyChars(buf + totalCount , p->offset + offset, len);
		}
		else
		{
			memcpy_s(buf + totalCount, len, mOriginalBuffer + offset, len );
		}
		
		offset = 0; //once only

		totalCount += len;

		if (len < p->length)  //no more data required
			break;

		//next ptr
		p = p->next.get();
	}

	return totalCount;
}

int Line::GetLineCount()
{
	//TODO change it when actual implementation comes
	return mNoOfChar;
}

int Line::InsertChar(char c, unsigned int index)
{
	int ret = 1;

	if (!mAddBuffer)
		return 0;
	//so that When deletion is done just after a insertion it should not use same descriptor
	mDeleteLastDescriptor = nullptr;

	//Get to the Index offset by iterating through the piece table.
	if (mPieceDescriptor == nullptr)
	{
		//create one when every thing is deleted
		mPieceDescriptor = make_shared<PieceDescriptor>();
		
	}
	shared_ptr<PieceDescriptor> pieceTable = mPieceDescriptor;
	shared_ptr<PieceDescriptor> prev = nullptr;

	int tempIndex = 0;

	while (pieceTable->length + tempIndex < index)
	{
		tempIndex += pieceTable->length;

		prev = pieceTable;
		//next raw pointer
		pieceTable = pieceTable->next;
		if (pieceTable == nullptr)
		{
			//error as index is too much
			return 0;
		}
	}

	if (pieceTable == mInsertLastDescriptor) //than we can may add to the add buffer directly
	{
		if ((tempIndex + pieceTable->length) == index)
		{			
			mAddBuffer->AddChar(c);
			++pieceTable->length;

			++mNoOfChar;

			return 1;
		}
	}
	//No else
	{
		int thirdLength = pieceTable->length;

		//making a new descriptor (First)
		pieceTable->length = index - tempIndex;
	
		//making a new descriptor (Third)
		shared_ptr<PieceDescriptor> third = make_shared<PieceDescriptor>();
		third->addBuffer = pieceTable->addBuffer;
		third->offset = pieceTable->offset + pieceTable->length;
		third->length = thirdLength - pieceTable->length;

		//making a new descriptor (second)
		shared_ptr<PieceDescriptor> second = make_shared<PieceDescriptor>();
		second->addBuffer = TRUE;
		second->offset = mAddBuffer->AddChar(c);;
		second->length = 1;
		
		third->next = pieceTable->next;
		
		if (pieceTable->length == 0)
		{
			if (prev)
			{
				prev->next = second;
			}
			else
			{
				mPieceDescriptor = second;
			}
		}
		else{
			pieceTable->next = second;
		}
		
		if (third->length == 0) //may be at the end of the block
		{
			second->next = third->next;
			third.reset();
		}
		else
			second->next = third;

		//save the last descriptor
		mInsertLastDescriptor = second;
	}
	++mNoOfChar;

	return ret;
}

int Line::DeleteChar(unsigned int index)
{
	int ret = 1;

	if (index > mNoOfChar)
		return 0;
	
	//so that When insert is done just after a deletion it should not use same descriptor
	mInsertLastDescriptor = nullptr;

	//Get to the Index offset by iterating through the piece table.
	shared_ptr<PieceDescriptor> pieceTable = mPieceDescriptor;
	shared_ptr<PieceDescriptor> prev = nullptr;

	int cumulativeIndex = 0;


	while (pieceTable->length + cumulativeIndex < (index + 1))
	{
		cumulativeIndex += pieceTable->length;

		prev = pieceTable;
		//next raw pointer
		pieceTable = pieceTable->next;
		if (pieceTable == nullptr)
		{
			//error as index is too much
			return 0;
		}
	}

	int offset = index - cumulativeIndex;

	if (offset == 0)
	{
		--pieceTable->length;
		if (!pieceTable->length)
		{
			//release this descriptor if 0
			if (prev)
			{
				prev->next = pieceTable->next;
				pieceTable->next = nullptr;
				pieceTable.reset();
			}
			else
			{
				mPieceDescriptor = pieceTable->next;
				pieceTable->next = nullptr;
				pieceTable.reset();
			}

			mDeleteLastDescriptor = nullptr;
		}
		else
		{
			++pieceTable->offset;
		}
	}
	else if (pieceTable->length + cumulativeIndex - 1 == index)
	{
		--pieceTable->length;

		if (!pieceTable->length)
		{
			//release this descriptor if 0
			prev->next = pieceTable->next;
			pieceTable->next = nullptr;
			pieceTable.reset();

			mDeleteLastDescriptor = nullptr;
		}
		else
		{
			mDeleteLastDescriptor = pieceTable;
		}
	}
	else
	{
		//split it
		shared_ptr<PieceDescriptor> second = make_shared<PieceDescriptor>();
		second->addBuffer = pieceTable->addBuffer;
		second->next = pieceTable->next;
		second->offset = pieceTable->offset + offset + 1;
		second->length = pieceTable->length - offset - 1;

		pieceTable->next = second;
		pieceTable->length = offset;

		mDeleteLastDescriptor = pieceTable;

	}

	--mNoOfChar;
	return ret;
}

int Line::InsertWord(char *word, unsigned int index)
{
	int ret = 0;

	return ret;
}
int Line::DeleteWord(unsigned int index)
{
	int ret = 0;

	return ret;
}

void Line::PrintTable()
{
	PieceDescriptorP p = mPieceDescriptor.get();

	while (p)
	{
		char buf[1024];
		sprintf_s(buf, "\naddB: %d --- off:%d ---- len:%d \n",p->addBuffer, p->offset, p->length);
		OutputDebugStringA(buf);
		p = p->next.get();
	}
}