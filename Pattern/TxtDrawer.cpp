#include "stdafx.h"
#include "TxtDrawer.h"
#include "Commdlg.h"

TxtDrawer::TxtDrawer(string fileName) :
mFilePath(fileName),
mCurrentStart(0),
mCurrentEnd(0),
mTotalLines(0),
mTotalVisibleLines(0),
mTotalVisbileChars(0),
mOldCurrentStart(-1),
mCurrentOffset(0),
mCurrentLine(0),
mStateChanged(TRUE),
mCaretLine(0),
mCaretOffset(0)
{
	mLexer = new TxtLexer();

	mLineBuffer = make_shared<LineBuffer>();

	//these file opening part needs to be done by the Document interface
	
	/*
	 *  can we defer the file reading for next part
	 */
	
	FILE *file;
	fopen_s(&file, mFilePath.c_str(), "r");
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *string = (char*)malloc(fsize+1);
	fread(string, fsize, 1, file);
	fclose(file);
	string[fsize] = '\n';
	
	/*const char *filepath = mFilePath.c_str();
	
	HANDLE file = CreateFileA(mFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	
	DWORD file_size;
	if (INVALID_HANDLE_VALUE != file)
	{
		file_size = GetFileSize(file, NULL);
		if (file_size == 0) {

			// Handle error 

		}

		char *string = (char*)malloc(file_size + 1);
		ReadFile(file, string, file_size, 0, 0);

		//string[file_size.QuadPart] = '\n';
		
		
	}
	*/

	//insert the texts
	mTotalLines = mLineBuffer->InsertData(string, fsize);
	
	//free the memory
	free(string);
	
}


TxtDrawer::~TxtDrawer()
{
}

void TxtDrawer::SetBoundingRect(Rect r)
{
	mBoundingRect = r;

	if (mCanvas)
		mCanvas.reset();
	mCanvas = make_shared<Canvas>(r.left, r.top, r.right - r.left, r.bottom - r.top);

	
	int lfHeight = -MulDiv(14, GetDeviceCaps(mCanvas->GetBitmap()->GetHDC(), LOGPIXELSY), 72);

	HFONT font = CreateFont(lfHeight, 8, 0, 0, 700, FALSE, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, L"Consolas");

	SelectObject(mCanvas->GetBitmap()->GetHDC(), font);

	mTotalVisibleLines = ComputeTotalVisibleLines(r.bottom - r.top, ComputeFontHeight()) - 1;
	mTotalVisbileChars = ComputeTotalCharsInALine(r.right - r.left, ComputeFontWidth()) - 1;

	mCanvas->GetBitmap()->CreateCaret(5, ComputeFontHeight());

	ChangeCaretToNewLine(0);
}

void TxtDrawer::OnResize(Rect r)
{

	//release old canvas
	if (mCanvas)
		mCanvas.reset();
	//new canvas
	mCanvas = make_shared<Canvas>(r.left, r.top, r.right - r.left, r.bottom - r.top);

	//bounding rect
	mBoundingRect = r;

	//set to -1 so that page can be redrawn
	mOldCurrentStart = -1;

	int lfHeight = -MulDiv(14, GetDeviceCaps(mCanvas->GetBitmap()->GetHDC(), LOGPIXELSY), 72);

	HFONT font = CreateFont(lfHeight, 8, 0, 0, 700, FALSE, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, L"Consolas");

	SelectObject(mCanvas->GetBitmap()->GetHDC(), font);

	int oldTotalVisibleLines = mTotalVisibleLines;
	int oldTotalVisbileChars = mTotalVisbileChars;

	mTotalVisibleLines = ComputeTotalVisibleLines(r.bottom - r.top, ComputeFontHeight()) - 1;
	mTotalVisbileChars = ComputeTotalCharsInALine(r.right - r.left, ComputeFontWidth()) - 1;

	mCanvas->GetBitmap()->ReleaseCraet();
	mCanvas->GetBitmap()->CreateCaret(5, ComputeFontHeight());

	//calculate the new offset and current line
	int mul;
	int oldOffset = mCurrentOffset + mCaretOffset;

	mul = oldOffset / mTotalVisbileChars;
	mCurrentOffset = mul * mTotalVisbileChars;
	mCaretOffset = oldOffset % mTotalVisbileChars;

	int oldCurrentLine = mCurrentStart + mCaretLine;

	mul = oldCurrentLine / mTotalVisibleLines;
	mCurrentStart = mul * mTotalVisibleLines;
	mCaretLine = oldCurrentLine % mTotalVisibleLines;

	// it dos not work so try to keep the lines around the mCaretLine 
	ChangeCaretToNewLine(mCaretLine);
}

void TxtDrawer::OnKeyDown(Key key)
{
	switch (key)
	{
		case KEY_DOWN:
		{
			if (mCurrentStart + mCaretLine < mTotalLines-1)
			{
				if (mCaretLine >= mTotalVisibleLines)
				{
					//mCaretLine--;
					mCurrentStart++;
					mStateChanged = TRUE;
					//change the caret to new line position will remain same
					ChangeCaretToNewLine(mCurrentStart + mCaretLine);
				}
				else
				{
					//change the caret to new line
					++mCaretLine;
					ChangeCaretToNewLine(mCurrentStart + mCaretLine);
				}
			}
			break;
		}
		case KEY_UP:
		{
			if (mCaretLine> 0)
			{
				--mCaretLine;
				//mCurrentStart--;
				//mStateChanged = TRUE;
				//change the caret to new line position will remain same
				ChangeCaretToNewLine(mCurrentStart + mCaretLine);
			}
			else
			{
				if (mCurrentStart)
				{
					mCurrentStart--;
					//change the caret to new line
					ChangeCaretToNewLine(mCurrentStart + mCaretLine);
				}
			}
			break;
		}
		case KEY_RIGHT:
		{
			/*
			 *    How to keep track of end of line
			 */
			int count = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine);

			if (mCurrentOffset+mCaretOffset < count)
			{
				if (mCaretOffset < mTotalVisbileChars)
				{
					//do not do anything
					++mCaretOffset;
					ChangeCaretToNewOffset(mCaretOffset);
				}
				else
				{
					++mCurrentOffset;
					mStateChanged = TRUE;
				}	
			}
			break;
		}
		case KEY_LEFT:
		{
			if (mCaretOffset > 0)
			{
				--mCaretOffset;
				ChangeCaretToNewOffset(mCaretOffset);
			}
			else
			{
				if (mCurrentOffset > 0)
				{
					--mCurrentOffset;
					mStateChanged = TRUE;
				}
				else
				{
					//go up one line(if present), but till the end
					if (mCurrentStart + mCaretLine > 0)
					{
						int count = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine -1);
						//mCurrentOffset = max(count - mTotalVisbileChars, 0) ;
						int mul = count / mTotalVisbileChars;
						mCurrentOffset = mTotalVisbileChars * mul;
						mCaretOffset = count % mTotalVisbileChars;
						OnKeyDown(Key::KEY_UP);
					}
				}
			}
			break;
		}
		case KEY_END:
		{
			int count = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine);
			//mCurrentOffset = max(count - mTotalVisbileChars, 0) ;
			int mul = count / mTotalVisbileChars;
			mCurrentOffset = mTotalVisbileChars * mul;
			mCaretOffset = count % mTotalVisbileChars;

			ChangeCaretToNewOffset(mCurrentOffset + mCaretOffset);
			break;
		}
		case KEY_RETURN:
		{
			AddNewLine(mCurrentStart + mCaretLine);
			break;
		}
		default:
			break;
	}
}

void TxtDrawer::OnCharKeyDown(unsigned int key)
{
	switch (key)
	{
		case 0x0A:

			// Process a linefeed. (Create a new line)
			//
			break;

		case 0x1B:

			// Process an escape. 

			break;

		case 0x09:

			// Process a tab
			break;

		case 0x0D:

			// Process a carriage return. 

			break;

		case 0x08:
			// Process a backspace.
			if (mCurrentOffset + mCaretOffset == 0)
			{
				if (mCurrentStart + mCaretLine == 0) //nothing is there and we r still deleteing
					return;
				int noChar = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine);
				
				if (noChar == 0)
				{
					//nothing is there : delete the line
					mLineBuffer->DeleteLine(mCurrentStart + mCaretLine);
					mTotalLines--;
					OnKeyDown(Key::KEY_LEFT);
				}
				else
				{
					char *buff = (char*)malloc(sizeof(char)*noChar);
					mLineBuffer->GetLine(mCurrentStart + mCaretLine, 0, noChar, buff);

					int j = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine - 1);

					int tempmCurrentStart = mCurrentStart;
					int tempmCaretLine = mCaretLine;

					OnKeyDown(Key::KEY_LEFT);

					for (int i = 0; i < noChar; i++)
					{
						mLineBuffer->InsertChar(tempmCurrentStart + tempmCaretLine - 1, buff[i], i + j);

					}
					//delete the line
					mLineBuffer->DeleteLine(tempmCurrentStart + tempmCaretLine);
					mTotalLines--;
				}
			}
			else
			{
				mLineBuffer->DeleteChar(mCurrentStart + mCaretLine, mCurrentOffset + mCaretOffset - 1); //we are going in the opposite direction
				OnKeyDown(Key::KEY_LEFT);
			}

			break;

		default:

			// Process displayable characters. 
			mLineBuffer->InsertChar(mCurrentStart + mCaretLine, (char)key, mCurrentOffset + mCaretOffset);
			mTotalLines = mLineBuffer->GetTotalLines();
			OnKeyDown(Key::KEY_RIGHT);
			break;

	}
}

void TxtDrawer::AddNewLine(int afterLineNo)
{
	if (afterLineNo < mTotalLines)
	{
		//get the char from the caret index to the end
		int linewidth = mLineBuffer->GetLineCount(afterLineNo);
		char *buff = (char*)malloc(sizeof(char)*linewidth);

		int charCopied = mLineBuffer->GetLine(afterLineNo, mCurrentOffset + mCaretOffset, linewidth, buff);

		if (charCopied > 0)
		{
			for (int i = 0; i < charCopied; i++)
			{
				//TODO we can make it more optimized by deleteing thw whole thing only once
				mLineBuffer->DeleteChar(afterLineNo, mCurrentOffset + mCaretOffset);
			}

			//insert it
			mLineBuffer->InsertLineAfter(afterLineNo, buff, charCopied);
		}
		else
		{
			//insert it
			mLineBuffer->InsertLineAfter(afterLineNo, nullptr, 0);
		}
		//increase the no of lines
		mTotalLines++;

		mCurrentOffset = 0;
		mCaretOffset = 0;
		
		if (buff)
			free(buff);
		OnKeyDown(Key::KEY_DOWN);
	}
	else
	{
		if (!afterLineNo)
		{
			//nothing is there on the screen so we put a blank line for the first line (here) 
			//and put another as second line
			mLineBuffer->InsertLineAfter(-1, nullptr, 0);
			mLineBuffer->InsertLineAfter(0, nullptr, 0);
			//increase the no of lines
			mTotalLines += 2;
			OnKeyDown(Key::KEY_DOWN);
		}
	}
}
extern HWND hWnd;
void TxtDrawer::Draw()
{

#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nTxtDrawer::Draw\n");
	DEBUGOUT(buf);
#endif

	/*
		how to draw a txt file

		may be we can get the file line by line.
		
	*/
	CHOOSEFONT cf;
	LOGFONT lf;
	HFONT hfont;

	// Initialize members of the CHOOSEFONT structure.  
	/*
	cf.lStructSize = sizeof(CHOOSEFONT);
	cf.hwndOwner = (HWND)NULL;
	cf.hDC = (HDC)NULL;
	cf.lpLogFont = &lf;
	cf.iPointSize = 0;
	cf.Flags = CF_SCREENFONTS;
	cf.rgbColors = RGB(0, 0, 0);
	cf.lCustData = 0L;
	cf.lpfnHook = (LPCFHOOKPROC)NULL;
	cf.lpTemplateName = (LPCWSTR)NULL;
	cf.hInstance = (HINSTANCE)NULL;
	cf.lpszStyle = (LPWSTR)NULL;
	cf.nFontType = SCREEN_FONTTYPE;
	cf.nSizeMin = 0;
	cf.nSizeMax = 0;
	*/
	//ChooseFont(&cf);

	//hfont = CreateFontIndirect(cf.lpLogFont);

	//int lfHeight = -MulDiv(16, GetDeviceCaps(mCanvas->GetBitmap()->GetHDC(), LOGPIXELSY), 96);
	//ReleaseDC(NULL, mCanvas->GetBitmap()->GetHDC());

	//hfont = CreateFontA(lfHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Times New Roman");

	//GetStockObject(DEFAULT_GUI_FONT)
	//SelectObject(mCanvas->GetBitmap()->GetHDC(), hfont);

	Rect r = { 0, 0, 0, 0 };
	//FillRect(hdcBmp)

	//SetMapMode(GetDC(NULL), MM_TEXT);
	
	int fHeight = ComputeFontHeight();

	//int fWidth = tm.tmAveCharWidth;

	char buf3[4096];

	if (!mStateChanged)
		return;
	RECT rr;
	rr.bottom = mBoundingRect.bottom;
	rr.top = mBoundingRect.top;
	rr.right = mBoundingRect.right;
	rr.left = mBoundingRect.left;
	InvalidateRect(hWnd, &rr, false);

	mCanvas->GetBitmap()->ClearRect(mBoundingRect);

	int i = 0; //mCurrentStart + 1; //we start at 1
	int ret = 0;

	while ((ret = mLineBuffer->GetLine(mCurrentStart + i, mCurrentOffset, 4096, buf3)) >= 0)
	{
		r.left = 0;
		r.right = mBoundingRect.right;
		r.top = (i)*fHeight;
		r.bottom = r.top + fHeight;

		i++;
		mCanvas->GetBitmap()->DrawText(buf3, ret, r);

		if (r.bottom >= mBoundingRect.bottom)
			break;
	}

	//mCanvas->GetBitmap()->ShowCaret(7*mCaretX, mCaretY*10);
	mOldCurrentStart = mCurrentStart;

	UpdateCaret();
	/*
	int i=1;
	int ret = 0;
	while((ret=mLineBuffer->GetLine(i, 4096, buf3))!=0)
	{
		r.left = 0;
		r.right = mBoundingRect.right;
		r.top = (i-1)*fHeight;
		r.bottom = r.top + fHeight;

		i++;
		mCanvas->GetBitmap()->DrawText(buf3, ret, r);
	}
	*/
#if 0
	int k = 0;
	int l = 0;
	char arr[256];
	//GetCharWidth32A(mCanvas->GetBitmap()->GetHDC(), 0, 255, arr);
	for (int i = 0; i < rr; i++,l++)
	{
		/*arr[0] = buf1[i];
		arr[1] = 0;

		SIZE a;
		GetTextExtentPoint32A(mCanvas->GetBitmap()->GetHDC(), arr, 1, &a);
		if (arr[0] == '\n')
		{
			//if (buf[++i] == '\n')
			{
				k = 0;
				l++;
				continue;
			}
		}
		if (arr[0] == '\t')
		{
			k += 8 * tm.tmAveCharWidth;
		}*/
		char line[4096];
		int j = 0;
		while(buf1[i] != '\n' && i<rr)
		{
			if (buf1[i] == '\t')
			{
				line[j++] = ' ';
				line[j++] = ' ';
				line[j++] = ' ';
				line[j++] = ' ';
			}
			line[j++] = buf1[i++];
		}

		SIZE a;
		GetTextExtentPoint32A(mCanvas->GetBitmap()->GetHDC(), line, j+1, &a);

		r.left = 0;
		r.right = k+a.cx;
		r.top = l*fHeight;
		r.bottom = r.top + fHeight;
		mCanvas->GetBitmap()->DrawText(line, j, r);
		
		/*k += fWidth;

		if ((k) >= mBoundingRect.right - mBoundingRect.left)
		{
			k = 0;
			l++;
		}*/
		if ((l*fHeight)>mBoundingRect.bottom - mBoundingRect.top)
			break;
	//		//TextOutA(hdcBmp, 0,0, buf1, strnlen_s(buf1,4096));//buf1, strnlen_s(buf1, 1024), &r, 1);
	}
	//mCanvas->GetBitmap()
	//mCanvas->GetBitmap()->DrawChar('c', r);
	//BitBlt(hdc, 0, 0, 1000, 500, hdcBmp, 0,0, SRCCOPY);
	
#endif

	mCanvas->GetBitmap()->Blit(mBoundingRect.left, mBoundingRect.top, mBoundingRect.right - mBoundingRect.left, mBoundingRect.bottom - mBoundingRect.top);
	

}

void TxtDrawer::SetCanvas(Canvas *canvas)
{

}

int TxtDrawer::ComputeTotalVisibleLines(int screenHeight, int fontHeight)
{
	return floor((screenHeight / fontHeight));
}

int TxtDrawer::ComputeTotalCharsInALine(int screenWidth, int fontWidth)
{
	return floor((screenWidth / fontWidth));
}

int TxtDrawer::ComputeFontHeight()
{
	TEXTMETRIC tm;
	GetTextMetrics(mCanvas->GetBitmap()->GetHDC(), &tm);


	return (tm.tmHeight + tm.tmExternalLeading);

	//int fWidth = tm.tmAveCharWidth;
}

int TxtDrawer::ComputeFontWidth()
{
	return 8;
}
void TxtDrawer::ChangeCaretToNewLine(int lineNo)
{
	int count = mLineBuffer->GetLineCount(lineNo);

	if (mCurrentOffset + mCaretOffset < count)
		count = mCurrentOffset + mCaretOffset;

	char buf[4096];
	mLineBuffer->GetLine(lineNo, 0, 4096, buf);
	int width = mCanvas->GetBitmap()->GetWidthOfTheString(buf, count);
	
	mCanvas->GetBitmap()->ShowCaret(width, mCaretLine * ComputeFontHeight());
}

void TxtDrawer::ChangeCaretToNewOffset(int offset)
{
	int count = mLineBuffer->GetLineCount(mCurrentStart + mCaretLine);
	
	if ((mCurrentOffset + offset) > count)
	{
		//change the caret to next lower line (go down)
		mCaretOffset = 0;
		mCurrentOffset = 0;
		OnKeyDown(Key::KEY_DOWN);
		//Draw();
	}
	else
	{
		mCanvas->GetBitmap()->ShowCaret(offset * ComputeFontWidth(), mCaretLine * ComputeFontHeight());
	}
}
void TxtDrawer::UpdateCaret()
{
}