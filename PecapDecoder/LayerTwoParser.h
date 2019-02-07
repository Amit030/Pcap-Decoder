#ifndef __LAYERTWOPARSER_H__
#define  __LAYERTWOPARSER_H__
#include "stdafx.h"
#include "Structures.h"


class CLayerTwoParser{
	packetHeader *m_pktHeader;
public:

	CLayerTwoParser();
	void performLevelTwoParsing(fstream *file, int * pos);
    void showLayerTwoData();
	packetHeader* getPacketHeader();

};


#endif