#pragma once

#include "stdafx.h"

class Lexer
{
public:
	Lexer();
	~Lexer();

	virtual string GetToken() = 0;

};

