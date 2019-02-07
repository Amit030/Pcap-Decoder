#ifndef __UDP_H__INCLUDED__
#define __UDP_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"

class CUDP{

    updHeader *m_udpHeader;
    int m_dataSize;
public:
    CUDP();
   void  parseUdpHeader(fstream *file,int pos);
   updHeader* getUdpHeader();
   void showUpdData();
   int getDataSize();
   ~CUDP();
};

#endif