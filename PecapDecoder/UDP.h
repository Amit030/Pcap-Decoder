#ifndef __UDP_H__INCLUDED__
#define __UDP_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"

class CUDP{

    updHeader *m_udpHeader;
    int m_dataSize;
    string m_packData;
public:
    CUDP();
   void  parseUdpHeader(fstream *file,int pos);
   updHeader* getUdpHeader();
   string getUpdData();
   int getDataSize();
   void  parseUdpData(fstream *file);
   ~CUDP();
};

#endif