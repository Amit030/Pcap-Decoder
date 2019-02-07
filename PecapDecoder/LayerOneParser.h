#ifndef __LAYERONEPARSER_H__INCLUDED__
#define __LAYERONEPARSER_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"


class CLayerOneParser{
 struct globalHeader *gHeader;

public:
	CLayerOneParser();
	void performLevelOneParsing(fstream *file, int * pos);
	globalHeader *getGlobalHeader();
    void showLayerOneData();
	~CLayerOneParser();

};

#endif