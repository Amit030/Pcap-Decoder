#ifndef __INTERNETVERSION_H__INCLUDED__
#define __INTERNETVERSION_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"


class CInternetVersion{
ipv4Header *ipHeader;
public:
	CInternetVersion();
	void parseIPV4Header(fstream *file);
	ipv4Header * getIPV4header();
    void showInternetVersionData();
	~CInternetVersion();

};

#endif