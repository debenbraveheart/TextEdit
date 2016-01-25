#include "stdafx.h"
#include "DocumentDrawerFactory.h"


DocumentDrawerFactory::DocumentDrawerFactory()
{
}


DocumentDrawerFactory::~DocumentDrawerFactory()
{
}


DocDrawer* DocumentDrawerFactory::CreateDocDrawer(string file)
{
#ifdef DEBUG
	char buf[1024];
	sprintf_s(buf, "\nDocumentDrawerFactory::CreateDocDrawer\n");
	DEBUGOUT(buf);
#endif
	return new TxtDrawer(file);
}