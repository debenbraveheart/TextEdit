#pragma once
#include "stdafx.h"

#define MAXBUF 1024
#define PTRSIZE sizeof(void*)
#define MAXBUFSIZE (MAXBUF - PTRSIZE)

typedef struct _PieceDescriptor
{
	shared_ptr<_PieceDescriptor> next;
	
	bool addBuffer; //True if add buffer or Original buffer
	unsigned offset;
	unsigned length;
	
	_PieceDescriptor()
	{
		next = nullptr;
		addBuffer = FALSE;
		offset = 0;
		length = 0;
	}

	~_PieceDescriptor()
	{
		if (next)
			next.reset();
	}
	
}PieceDescriptor, *PieceDescriptorP;


class AddBuffer
{
	enum _CAPACITY{
		BUFF_CAPACITY = 1024
	};
private:
	char *mAddBuffer; //linear buffer. Use calloc for re initialization
	int mCapacity;
	int mFilled;

	void ResizeBuffer()
	{
		mAddBuffer = (char*)calloc(mCapacity + BUFF_CAPACITY, sizeof(char));
		mCapacity += BUFF_CAPACITY;
	}
public:
	AddBuffer()
	{
		mAddBuffer = (char*)malloc(sizeof(char) * BUFF_CAPACITY);
		mCapacity = BUFF_CAPACITY;
		mFilled = 0;
	}
	~AddBuffer()
	{
		if (mAddBuffer)
			free(mAddBuffer);
		mAddBuffer = nullptr;
	}
	/*   return the index at which the char was inserted*/
	unsigned int AddChar(char c)
	{
		if (mFilled == mCapacity)
		{
			ResizeBuffer();
		}

		mAddBuffer[mFilled] = c;

		mFilled++;

		//return the index of the char stored;
		return mFilled - 1;
	}
	void CopyChars(char *dest, unsigned int offset, int length)
	{
		if ((offset + length) > (mFilled))
			return;

		memcpy_s(dest, length, mAddBuffer+offset, length);
	}
};


class Line
{
	shared_ptr<Line> mNextLinePtr;
	char *mOriginalBuffer; //orginial buffer

	char *mVisibleBuffer;

	shared_ptr<AddBuffer> mAddBuffer; //Add Buffer

	int mNoOfChar;

	shared_ptr<PieceDescriptor> mPieceDescriptor;
	shared_ptr<PieceDescriptor> mInsertLastDescriptor;
	shared_ptr<PieceDescriptor> mDeleteLastDescriptor;

	//get the data from 
public:
	Line();
	~Line();

	void Copy(char *buff, int count);

	int GetLine(int count, int startOffset, char *buf);

	int GetLineCount();

	int InsertChar(char c, unsigned int index);
	int DeleteChar(unsigned int index);
	int InsertWord(char *word, unsigned int index);
	int DeleteWord(unsigned int index);

	void ResizeAddBuffer(unsigned int buufSize);
	void PrintTable();
};

