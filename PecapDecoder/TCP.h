#ifndef __TCP_H__INCLUDED__
#define __TCP_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"

class CTCP{
    tcpHeader *m_tcpHeader;
    int m_headerLen;
    int m_dataSize;
public:
    CTCP();
   void parseTcpHeader(fstream *file, int totalLen);
    tcpHeader * getTcpHeader();
    void showTcpData();
    int getTcpHeaderLength();
     int getDataSize();
     void matchHttp();
    ~CTCP();


};

#endif