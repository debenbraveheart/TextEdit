#pragma once

#include "stdafx.h"
#include "DocDrawer.h"
#include "TxtDrawer.h"

class DocumentDrawerFactory
{
private:
	DocumentDrawerFactory();
public:
	~DocumentDrawerFactory();

	static DocDrawer* CreateDocDrawer(string file);
};

