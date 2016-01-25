#pragma once

#include "Lexer.h"

class TxtLexer : public Lexer
{
public:
	TxtLexer();
	~TxtLexer();
	string GetToken();
};

