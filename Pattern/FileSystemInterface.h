#pragma once

#include "stdafx.h"
#include "FileInterface.h"

class FileSystemInterface
{
public:
	FileSystemInterface();
	~FileSystemInterface();

	
	virtual  char* GetFileSystemName() = 0;
	virtual void Init() = 0 ;
	virtual FileInterface GetFileInterface() = 0;

};

