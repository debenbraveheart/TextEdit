#pragma once

#include "stdafx.h"
#include "Parser.h"
#include "Lexer.h"
#include "DocDrawer.h"
#include "DocumentDrawerFactory.h"

class Document
{
private:
	string mFileName;
	string mPath;

	Parser* mParser;
	bool mParsed;

	Lexer* mLexer;
	bool mLexerFinshed;

	void Init();


public:
	Document();
	Document(string file);

	void SetFilePath(string path);
	string GetFilePath();

	~Document();
};

