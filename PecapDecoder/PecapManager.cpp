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

void CPcapManager::readPcapFile(string path){
    m_file->open(path, ios::in | ios::binary);
    if( !m_file->is_open()) {
        cout<<"Not able to read specified file"<<endl;
        getchar();
    }
    return;
}
void CPcapManager ::parsePecapFile(){
    //Read global header
    m_file->seekg(0,ios::end);
    long l_fileSize=m_file->tellg();
    m_file->seekg(0,ios::beg);
    m_layerOneParser->performLevelOneParsing(m_file,& m_offset);
    m_layerOneParser->showLayerOneData();



    int count=0;
    while(   m_offset <l_fileSize  ){

        m_file->seekg(m_offset ,ios::beg);
        if(m_file->tellg() >= 0)
        {
            count++;
            // cout<<"Packet Number : "<< count<<"     Current Poisition   : "<<m_offset<<endl<<endl;
            // cout<<"Current poisition : "<<m_file->tellg()<<endl;
            m_layerTwoParser->performLevelTwoParsing(m_file,& m_offset); 
            m_layerThreeParser->performLevelThreeParsing(m_file,m_layerOneParser->getGlobalHeader()->linktype,&m_offset); 
            unsigned short frameType=m_layerThreeParser->getEtherHeader()->FrameType;
            if(frameType != 0xdd86)
                ipVersion->parseIPV4Header(m_file);
            else
                continue;
            if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP)
                tcpHeader->parseTcpHeader(m_file,m_offset);

            //    cout<<"Next packet Starting poistion (Expected) : "<<m_offset<<endl  <<endl<<endl;
        }
        if(ipVersion->getIPV4header()->Protocol==IPPROTO_UDP)
            udpHeader->parseUdpHeader(m_file,m_offset);


        writePacket();


    }
    dumpDataInFile();
    getchar();
}




void CPcapManager:: showLogs(){

    m_layerTwoParser->showLayerTwoData();
    m_layerThreeParser->showLayerThreeData();
    ipVersion->showInternetVersionData();

}

string  CPcapManager:: generateKey(){
    if(ipVersion !=NULL){
        string srcIP=inet_ntoa(ipVersion->getIPV4header()->ip_src);
        string destIP=inet_ntoa(ipVersion->getIPV4header()->ip_dst);
        string ipKey;
        string portKey;
        if(srcIP>destIP){
            ipKey= srcIP+DELIMETER+destIP+DELIMETER;
        }
        else{
            ipKey= destIP+DELIMETER+srcIP+DELIMETER;
        }
        if(ipVersion->getIPV4header()->Protocol==IPPROTO_TCP){
            string srcPort=convertToString(ntohs(tcpHeader->getTcpHeader()->SrcPort));
            string destPort=convertToString(ntohs(tcpHeader->getTcpHeader()->DstPort));

            if(srcPort>destPort){
                portKey=srcPort+DELIMETER+destPort+TCP_PROTOCOL;
            }
            else{
                portKey=destPort+DELIMETER+srcPort+ TCP_PROTOCOL;
            }
        }
        return ipKey+portKey;
    }

}
template <class T>
string  CPcapManager ::convertToString(T a){
    std::stringstream ss;
    ss << a;
    return ss.str();
}
void CPcapManager :: writePacket(){
    if(ipVersion !=NULL && (ipVersion->getIPV4header()->Protocol==IPPROTO_TCP || ipVersion->getIPV4header()->Protocol==IPPROTO_UDP )){
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
            }


        }



    }

}

void CPcapManager :: dumpDataInFile(){

    for (itr = m_sessionMap.begin(); itr != m_sessionMap.end(); itr++) 
    {  
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
        //  m_file->write((itr->second.clientIP).c_str(),strlen((itr->second.clientIP).c_str()));

        /*

        cout <<"FILE NAME  :   " <<itr->first <<endl;
        cout<<"Client IP : "<<itr->second.clientIP<<endl;
        cout<<"Server IP : "<<itr->second.serverIP<<endl;
        cout<<"Server Port : "<<itr->second.serverPort<<endl;
        cout<<"Client POrt : "<<itr->second.clientPort<<endl;
        cout<<"Rx  : "<<itr->second.Rx<<endl;
        cout<<"Tx  :  "<<itr->second.Tx<<endl;
        cout <<"Number of packets in session : "<<itr->second.packetCount<<endl;
        cout<<"Protocol  : "<<itr->second.transportProtocol<<endl;
       
       */
        cout<<endl<<"____________________________________________________________"<<endl<<endl;

    }



} 