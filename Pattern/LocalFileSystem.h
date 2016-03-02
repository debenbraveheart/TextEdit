#pragma once
#include "FileSystemInterface.h"
class LocalFileSystem :
	public FileSystemInterface
{
private:
	char* mFileSystemFileName;
public:
	LocalFileSystem();
	~LocalFileSystem();

	char* LocalFileSystem::GetFileSystemName();
};

