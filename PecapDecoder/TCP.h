#ifndef __TCP_H__INCLUDED__
#define __TCP_H__INCLUDED__

#include "stdafx.h"
#include "Structures.h"
#include <string>
using namespace std;

class CTCP{
    tcpHeader *m_pTcpHeader;
    int m_headerLen;
    int m_dataSize;
    string m_packData;
    void parseTcpData(fstream *file);
    void print_payload(const u_char *payload, int len);
    void print_hex_ascii_line(const u_char *payload, int len, int offset);
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