#include <cstdio>
#include <iostream>
#include "Line.h"
#include "LineBuffer.h"

using namespace std;
int main()
{
	
	char buf[1024];
	sprintf_s(buf, "A large span of text");

	Line *l = new Line();
	l->Copy(buf, 20);
	l->DeleteChar(2);
	l->DeleteChar(2);
	l->DeleteChar(2);
	l->DeleteChar(2);
	l->DeleteChar(2);
	l->DeleteChar(2);
	l->InsertChar( 'e', 10);
	l->InsertChar('n', 11);
	l->InsertChar('g', 12);
	l->InsertChar('l', 13);
	l->InsertChar('i', 14);
	l->InsertChar('s', 15);
	l->InsertChar('h', 16);
	l->InsertChar(' ', 17);
	l->DeleteChar(13);
	l->DeleteChar(13);
	l->PrintTable();
}