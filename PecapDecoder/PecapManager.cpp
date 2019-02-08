#include "stdafx.h"
#include"PecapManager.h"

CPcapManager::CPcapManager(){
    m_offset=0;
    m_sessionCounter=0;
    m_file= new fstream();
    m_layerOneParser= new CLayerOneParser();
    m_layerTwoParser= new CLayerTwoParser();
    m_layerThreeParser = new CLayerThreeParser();
    ipVersion= new CInternetVersion();
    tcpHeader = new CTCP();
    udpHeader= new CUDP();


}


/**
* Function         :   CPcapManager :: readPcapFile
* Description      :   read the file from provide path 
* Arguments        :   path of the file
* Return-type      :   void
*/

void CPcapManager::readPcapFile(string path){
    m_file->open(path, ios::in | ios::binary);
    if( !m_file->is_open()) {
        cout<<"Not able to read specified file"<<endl;
        getchar();
    }
    return;
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
    m_file->seekg(0,ios::end);
    long l_fileSize=m_file->tellg();
    m_file->seekg(0,ios::beg);
    m_layerOneParser->performLevelOneParsing(m_file,& m_offset);

    int count=0;
    while(   m_offset <l_fileSize  ){

        m_file->seekg(m_offset ,ios::beg);
        if(m_file->tellg() >= 0)
        {
            count++;
            m_layerTwoParser->performLevelTwoParsing(m_file,& m_offset); 
            m_layerThreeParser->performLevelThreeParsing(m_file,m_layerOneParser->getGlobalHeader()->linktype,&m_offset); 
            unsigned short frameType=m_layerThreeParser->getEtherHeader()->FrameType;
            if(frameType != 0xdd86)
                ipVersion->parseIPV4Header(m_file);
            else
                continue;
            if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP)
                tcpHeader->parseTcpHeader(m_file,m_offset);

            if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP)
                udpHeader->parseUdpHeader(m_file,m_offset);

            writePacket();
        }

    }
    dumpDataInFile();

}




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
                if(srcIP==itr->second.clientIP)
                    itr->second.Tx +=RxTx;          
                else
                    itr->second.Rx += RxTx;
            }else if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP){

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
                m_session _sdetails (destIP,srcIP,destPort,srcPort,0,RxTx,protocol);
                m_sessionMap.insert(std::pair<string,sessionInfo>(key, _sdetails));
            }

            else{
                m_session _sdetails (srcIP,destIP,srcPort,destPort,RxTx,0,protocol);
                m_sessionMap.insert(std::pair<string,sessionInfo>(key, _sdetails));
                if(tcpHeader->getTcpHeader()->th_flags == TH_RST ||tcpHeader->getTcpHeader()->th_flags== TH_FYN_AKW ){
                    writeAndRemoveSession(key);
                }
            }

        }

    }

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
        cout<<endl<<"Writing Session file  :"<<count<<" .."<<endl<<endl;

        file.open (itr->first.c_str());
        file <<"Client IP : "<<itr->second.clientIP.c_str()<<endl;
        file <<"Server IP : "<<itr->second.serverIP.c_str()<<endl;
        file <<"Server Port : "<<itr->second.serverPort.c_str()<<endl;
        file <<"Client Port : "<<itr->second.clientPort.c_str()<<endl;
        file <<"Rx  : "<<itr->second.Rx<<endl;
        file <<"Tx  :  "<<itr->second.Tx<<endl;
        file <<"Number of packets in session : "<<itr->second.packetCount<<endl;
        file <<"Protocol  : "<<itr->second.transportProtocol.c_str()<<endl;
        file.close();
        m_sessionMap.erase(itr);

    }

} 

void  CPcapManager :: writeAndRemoveSession(string key){
    auto itr = m_sessionMap.find(key);
    if(itr != m_sessionMap.end()){
        file.open (itr->first.c_str());
        file <<"Client IP : "<<itr->second.clientIP.c_str()<<endl;
        file <<"Server IP : "<<itr->second.serverIP.c_str()<<endl;
        file <<"Server Port : "<<itr->second.serverPort.c_str()<<endl;
        file <<"Client Port : "<<itr->second.clientPort.c_str()<<endl;
        file <<"Rx  : "<<itr->second.Rx<<endl;
        file <<"Tx  :  "<<itr->second.Tx<<endl;
        file <<"Number of packets in session : "<<itr->second.packetCount<<endl;
        file <<"Protocol  : "<<itr->second.transportProtocol.c_str()<<endl;
        file.close();
        m_sessionMap.erase(itr);
    }

}