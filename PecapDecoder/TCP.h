#ifndef __TCP_H__INCLUDED__
#define __TCP_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"
#include <string>
using namespace std;

class CTCP{
    tcpHeader *m_tcpHeader;
    int m_headerLen;
    int m_dataSize;
    string m_packData;
    void parseTcpData(fstream *file);
public:
    CTCP();
   void parseTcpHeader(fstream *file, int totalLen);
    tcpHeader * getTcpHeader();
    void showTcpData();
    int getTcpHeaderLength();
     int getDataSize();
     string getTcpData();
     void matchHttp();
    ~CTCP();


};

#endif