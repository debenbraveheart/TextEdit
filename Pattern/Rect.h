#pragma once

#include <cstdio>

typedef struct _Rect 
{
	unsigned int left;
	unsigned int top;
	unsigned int right;
	unsigned int bottom;
}Rect;

typedef Rect* PRect;