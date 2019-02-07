#ifndef __LAYERTHREEPARSER_H__
#define __LAYERTHREEPARSER_H__
#include "stdafx.h"
#include "Structures.h"


#define ETHERNET_CAPTURES 0x01
#define LINUX_COOKED_CAPTURES 0x71





class CLayerThreeParser{
	etherHeader *m_ethHeader;
	Linux_cooked_capture *linuxHeader;
public:

	CLayerThreeParser();
	void performLevelThreeParsing(fstream *file,u_int linkType,int * pos);
    void showLayerThreeData();
	etherHeader* getEtherHeader();

};


#endif