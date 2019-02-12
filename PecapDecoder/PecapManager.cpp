#include "stdafx.h"
#include"PecapManager.h"




/**
* Function       :  CPcapManager::CPcapManager
* Description    :  Default Constructor/ initializing the heades and member variables
* Arguments      :  None
* Return-type    :  Node 
*/
CPcapManager::CPcapManager(){
    m_offset=0;
    m_sessionCounter=0;
    m_pFile= new fstream();
    m_pFilesInDir = new vector<string>;
    m_layerOneParser= new CLayerOneParser();
    m_layerTwoParser= new CLayerTwoParser();
    m_layerThreeParser = new CLayerThreeParser();
    ipVersion= new CInternetVersion();
    tcpHeader = new CTCP();
    udpHeader= new CUDP();


}


/**
* Function       :  CPcapManager::~CPcapManager
* Description    :  Destructor/De-intilization
* Arguments      :  None
* Return-type    :  Node 
*/
CPcapManager::~CPcapManager(){
    delete m_pFile;
    delete m_pFilesInDir;
    delete m_layerOneParser;
    delete  m_layerTwoParser;
    delete m_layerThreeParser;
    delete  ipVersion;
    delete tcpHeader;
    delete udpHeader;
}

/**
* Function         :   CPcapManager :: readPcapFile
* Description      :   read the file from provide path 
* Arguments        :   path of the file, output dir path
* Return-type      :   void
*/

void CPcapManager::readPcapFile(string path,string outpath){
    outputPath=outpath+"\\";
    createOutputDir(outpath);
    get_all_files_names_within_folder(path);
    int count =1;
    for (auto i = m_pFilesInDir->begin(); i != m_pFilesInDir->end(); ++i){
        cout<<"\n\nReading file  : "<< count << endl<<endl;
        cout<<"Current File Name : "<<*i<<endl;

        string filePath = path+"\\"+ *i;
        cout<<"\nPath of packet : "<<filePath<<endl;
        m_pFile->open(filePath, ios::in | ios::binary);
        if( !m_pFile->is_open()) {
            cout<<"Not able to read specified file"<<endl;
            getchar();
        }else{
            parsePecapFile();
        }

        m_pFile->close();count++;
         dumpDataInFile();
    }

    cout <<"\n\n  Done Reading Data \n\n"<<endl;
  
    return;
}


/**
* Function         :   CPcapManager ::createOutputDir
* Description      :   create a dir at the mentioned path in fun arg
* Arguments        :   string : output dir path
* Return-type      :   void
*/
void CPcapManager ::createOutputDir(string outpath){
    std::wstring widestr = std::wstring(outpath.begin(), outpath.end());
    CreateDirectory(widestr.c_str(), NULL);

}

/**
* Function         :   CPcapManager :: parsePecapFile
* Description      :   parse pecap file structure vy structure
                       till EOF  and call futher functions for futher processing.
* Arguments        :   No arguments
* Return-type      :   void
*/

void CPcapManager ::parsePecapFile(){
    //Read global header
    m_pFile->seekg(0,ios::end);
    long l_fileSize=m_pFile->tellg();
    m_pFile->seekg(0,ios::beg);
    m_layerOneParser->performLevelOneParsing(m_pFile,& m_offset);

    int count=0;
    while(   m_offset <l_fileSize  ){

        m_pFile->seekg(m_offset ,ios::beg);
        if(m_pFile->tellg() >= 0)
        {
            count++;
            m_layerTwoParser->performLevelTwoParsing(m_pFile,
                &m_offset); 
            
            m_layerThreeParser->performLevelThreeParsing(
                m_pFile,
                m_layerOneParser->getGlobalHeader()->linktype,
                &m_offset); 

            unsigned short frameType=m_layerThreeParser->getEtherHeader()->FrameType;
            if(frameType != 0xdd86)
                ipVersion->parseIPV4Header(m_pFile);
            else
                continue;
            if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP)
                tcpHeader->parseTcpHeader(m_pFile,m_offset);

            if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP)
                udpHeader->parseUdpHeader(m_pFile,m_offset);

            writePacket();
        }

    }


}


/**
* Function       :  CPcapManager :: showLogs
* Description    :  shows the logs of all classes
* Arguments      :  None
* Return-type    :  void 
*/
void CPcapManager:: showLogs(){
    m_layerOneParser->showLayerOneData();
    m_layerTwoParser->showLayerTwoData();
    m_layerThreeParser->showLayerThreeData();
    ipVersion->showInternetVersionData();

}


/**
* Function       :  CPcapManager :: convertToString
* Description    :  Template type function convert the provide arugument into string
* Arguments      :  Template type
* Return-type    :  string 
*/

template <class T>
string  CPcapManager ::convertToString(T a){
    std::stringstream ss;
    ss << a;
    return ss.str();

}


/**
* Function       :  CPcapManager :: writePacket
* Description    :  This function will generate key for every packet/Session 
                    (packets of same session will have same key) 
                    and after that it will serach for the key in the hashmap. 
                    If found it will update the data (RxTx) othereise it will 
                    create a new entry for the same session.

* Arguments      :   No Arguments
* Return-type    :   Void
*/

void CPcapManager :: writePacket(){
     cout<<endl<<"Parsing pcap file  :"<<endl;
    if( (ipVersion != NULL) && 
        ( (ipVersion->getIPV4header()->Protocol == IPPROTO_TCP) || 
        ( ipVersion->getIPV4header()->Protocol==IPPROTO_UDP ) ))
    {
        string srcIP=inet_ntoa(ipVersion->getIPV4header()->ip_src);
        string destIP=inet_ntoa(ipVersion->getIPV4header()->ip_dst);
        string ipKey,portKey,srcPort,destPort,protocol;
        int RxTx;

        if(srcIP>destIP)
            ipKey= srcIP+DELIMETER+destIP+DELIMETER;       
        else
            ipKey= destIP+DELIMETER+srcIP+DELIMETER;

        if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP){
            srcPort=convertToString(ntohs(tcpHeader->getTcpHeader()->SrcPort));
            destPort=convertToString(ntohs(tcpHeader->getTcpHeader()->DstPort));
            protocol=TCP_PROTOCOL;
            RxTx=tcpHeader->getDataSize();

            if(srcPort>destPort)
                portKey=srcPort+DELIMETER+destPort+TCP_PROTOCOL;            
            else
                portKey=destPort+DELIMETER+srcPort+ TCP_PROTOCOL;

        }else if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP){
            srcPort=convertToString(ntohs(udpHeader->getUdpHeader()->source_port));
            destPort=convertToString(ntohs(udpHeader->getUdpHeader()->dest_port));
            protocol=UDP_PROTOCOL;
            RxTx= udpHeader->getDataSize(); 
            if(srcPort>destPort)
                portKey=srcPort+DELIMETER+destPort+UDP_PROTOCOL;            
            else
                portKey=destPort+DELIMETER+srcPort+ UDP_PROTOCOL;
        }
        string key =ipKey+portKey;


        auto itr = m_sessionMap.find(key);
        if(itr != m_sessionMap.end()){
            // cout<<"Key Available Updating information"<<endl;
            if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP){
                itr->second.data.push_back(tcpHeader->getTcpData());
                if(srcIP==itr->second.clientIP)
                    itr->second.Tx +=RxTx;          
                else
                    itr->second.Rx += RxTx;
            }else if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP){
                itr->second.data.push_back(udpHeader->getUpdData());
                if(srcIP==itr->second.clientIP)
                    itr->second.Tx += RxTx;       
                else
                    itr->second.Rx += RxTx;
            }
            itr->second.packetCount+=1;
        }
        else{
            //   cout<<"Add new key to data haspmap" <<endl;
            if(tcpHeader->getTcpHeader()->th_flags == TH_SYN_ACK)  {           
                m_session _sdetails (destIP,srcIP,destPort,srcPort,0,RxTx,protocol,tcpHeader->getTcpData());
                m_sessionMap.insert(std::pair<string,sessionInfo>(key, _sdetails));
            }

            else{
                m_session _sdetails (srcIP,destIP,srcPort,destPort,RxTx,0,protocol,udpHeader->getUpdData());
                m_sessionMap.insert(std::pair<string,sessionInfo>(key, _sdetails));
                if(tcpHeader->getTcpHeader()->th_flags == TH_RST ||tcpHeader->getTcpHeader()->th_flags== TH_FYN_AKW ){
                writeAndRemoveSession(key);
                }
            }

        }

    }// if( (ipVersion != NULL) && ...

}
/**
* Function      :  CPcapManager :: dumpDataInFile
* Description   :  Write the data in disk present in the  hashmap and clear
                   the data from hashmap
* Arguments     :  No Arguments
* Return-type   :  Void
*/
void CPcapManager :: dumpDataInFile(){
    int count=0;
 

    for (itr = m_sessionMap.begin(); itr != m_sessionMap.end(); itr++) 
    { 
        count++;
        cout<<endl<<"Parsing pcap file  :"<<endl;
        file.open ((outputPath+itr->first).c_str());
        file <<"Client IP : "<<itr->second.clientIP.c_str()<<endl;
        file <<"Server IP : "<<itr->second.serverIP.c_str()<<endl;
        file <<"Server Port : "<<itr->second.serverPort.c_str()<<endl;
        file <<"Client Port : "<<itr->second.clientPort.c_str()<<endl;
        file <<"Rx  : "<<itr->second.Rx<<endl;
        file <<"Tx  :  "<<itr->second.Tx<<endl;
        file <<"Number of packets in session : "<<itr->second.packetCount<<endl;
        file <<"Protocol  : "<<itr->second.transportProtocol.c_str()<<endl;
        file<<"__________________________DATA_____________________________\n"<<endl;
        for (int i=0; i<itr->second.data.size(); i++)     {
           if(itr->second.data[i] !="")
            file << itr->second.data[i] << "\n";
         
        } 
      
        file.close();
        //m_sessionMap.erase(itr->first);

    }
    m_sessionMap.clear();
} 

void  CPcapManager :: writeAndRemoveSession(string key){
  
    auto itr = m_sessionMap.find(key);

    if(itr != m_sessionMap.end()){
        file.open ((outputPath+key).c_str());
        file <<"Client IP : "<<itr->second.clientIP.c_str()<<endl;
        file <<"Server IP : "<<itr->second.serverIP.c_str()<<endl;
        file <<"Server Port : "<<itr->second.serverPort.c_str()<<endl;
        file <<"Client Port : "<<itr->second.clientPort.c_str()<<endl;
        file <<"Rx  : "<<itr->second.Rx<<endl;
        file <<"Tx  :  "<<itr->second.Tx<<endl;
        file <<"Number of packets in session : "<<itr->second.packetCount<<endl;
        file <<"Protocol  : "<<itr->second.transportProtocol.c_str()<<endl;
                file<<"__________________________DATA_____________________________"<<endl;
        for (int i=0; i<itr->second.data.size(); i++)     {
            file << itr->second.data[i] << "\n";
        } 
      
        file.close();
        m_sessionMap.erase(itr);
    }

}

vector<string>* CPcapManager :: get_all_files_names_within_folder(string folder){

    string search_path = folder + "/*.pcap*";
    WIN32_FIND_DATA fd; 
    std::wstring widestr = std::wstring(search_path.begin(), search_path.end());
    HANDLE hFind = :: FindFirstFile(widestr.c_str(), &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do { 
            // read all (real) files in current folder
            // , delete '!' read other 2 default folder . and ..
            if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {

                wstring ws(fd.cFileName);
                string str(ws.begin(), ws.end());
                m_pFilesInDir->push_back(str);
            }
        }while(::FindNextFile(hFind, &fd)); 
        ::FindClose(hFind); 
    } 
    return m_pFilesInDir;
}