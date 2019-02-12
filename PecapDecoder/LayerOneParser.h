#ifndef __LAYERONEPARSER_H__INCLUDED__
#define __LAYERONEPARSER_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"


class CLayerOneParser{
 struct globalHeader *m_pGlobalHeader;

public:
	CLayerOneParser();
    ~CLayerOneParser();
	void performLevelOneParsing(fstream *file, int * pos);
	globalHeader *getGlobalHeader();
    void showLayerOneData();
	

};

#endif