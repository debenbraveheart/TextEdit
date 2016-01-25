#pragma once


#include <cstdio>
#include <debugapi.h>

#define DEBUG 1

#ifdef DEBUG
#define DEBUGOUT(a)				\
	{							\
	    OutputDebugStringA(a);\
	}
#else
#dfine DEBUGOUT(a)
#endif