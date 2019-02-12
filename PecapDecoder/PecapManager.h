#ifndef __PECAPMANAGER_H__INCLUDED__
#define __PECAPMANAGER_H__INCLUDED__

#include "stdafx.h"
#include "LayerOneParser.h"
#include "LayerTwoParser.h"
#include "LayerThreeParser.h"
#include "InternetVersion.h"
#include "TCP.h"
#include "UDP.h"
#include<string>
#include<sstream>
#include <unordered_map> 
#include <Windows.h>



#define TCP_PROTOCOL TCP
#define UDP_PROTOCOL UDP

#define TCP_PROTOCOL "_TCP"
#define UDP_PROTOCOL "_UDP"
#define DELIMETER    "_"
using namespace std;

class CPcapManager{  

    fstream *m_file;
    int m_offset;
    string outputPath;
    int m_sessionCounter;  vector<string>* m_filesInDir;
    unordered_map<string, sessionInfo> m_sessionMap; 
    unordered_map<std::string, sessionInfo>::const_iterator itr;
    CLayerOneParser *m_layerOneParser;
    CLayerTwoParser *m_layerTwoParser;
    CLayerThreeParser *m_layerThreeParser;
    CInternetVersion *ipVersion;
    CTCP *tcpHeader;
    CUDP *udpHeader;
    ofstream file;
    void showLogs();
    int getSeekLength();
    int getSeekLengthUdp();
    void Test();
    void  writePacket();
    void dumpDataInFile();
    void createOutputDir(string outpath);
    void  writeAndRemoveSession(string key);
    template <class T>
    string convertToString( T a);
    string generateKey();
    vector<string>* get_all_files_names_within_folder(string folder);
    void parsePecapFile();

public:
    CPcapManager();
    void readPcapFile(string path,string outpath);
    ~CPcapManager();

};


#endif