#include "stdafx.h"
#include "TCP.h"

CTCP::CTCP(){
    m_tcpHeader= new tcpHeader();
    m_dataSize=0;
}

void CTCP::parseTcpHeader(fstream *file,int totalLen){
    try{
        file->read((char*)m_tcpHeader,sizeof(tcpHeader)); }
    catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

     m_dataSize = totalLen - file->tellg();
 
    file->seekg((m_tcpHeader->HeaderLen >> 2)-sizeof(tcpHeader),ios::cur);
    parseTcpData(file);


}

tcpHeader * CTCP:: getTcpHeader(){
    return m_tcpHeader;
}
int  CTCP:: getDataSize(){
    return m_dataSize;
}

string CTCP:: getTcpData(){

   if(m_packData=="")
       return "";
   else{
   return m_packData;
   }
}

void CTCP:: showTcpData(){


    char temp = m_tcpHeader->HeaderLen;
    int headerLen = 0xF0 & temp;
    headerLen = headerLen >> 4;
    cout<<endl<<"-----------TCP HEADER-------------"<<endl<<endl;
    cout<<"Src port  : "<<ntohs(m_tcpHeader->SrcPort)<<endl;
    cout<<"Dest port : "<<ntohs(m_tcpHeader->DstPort)<<endl;
    cout<<"Acknowledge Number : "<<m_tcpHeader->AckNO<<endl;
    cout<<"Header Len  : "<< headerLen*4<<endl;
    cout<<"Window  : "<<ntohs(m_tcpHeader->Window)<<endl;
    cout<<"Checksum  : "<<m_tcpHeader->Checksum<<endl;
    cout<<"UrgentPointer : "<<m_tcpHeader->UrgentPointer<<endl;
    
    cout<<"______________________________TCP FLAG________________________________"<<endl;
    switch(m_tcpHeader->th_flags){
    case TH_SYN:
        cout<<"SYN FLHG"<<endl;
        break;  case TH_RST:
        cout<<"RST FLHG"<<endl;
        break;  case TH_PUSH:
        cout<<"PUSH FLHG"<<endl;
        break;  case TH_ACK:
        cout<<"ACK FLHG"<<endl;
        break;
        case TH_FIN:
        cout<<"TH_FIN"<<endl;
        break;
        case TH_SYN_ACK:
        cout<<"TH_SYN_ACK"<<endl;
        break;
       

    }

}


int CTCP:: getTcpHeaderLength(){
    return sizeof(tcpHeader);
}
CTCP::~ CTCP(){
    delete m_tcpHeader;
}


void  CTCP :: parseTcpData(fstream *file){

    if(m_dataSize !=0){
        char tempBuf[4];
        m_packData="";
        std::string methodBuf;
        std::string l_urlBuf;
        std::string l_hostBuf;
        std::string l_uaBuf;
        file->read(tempBuf,4);
        file->seekg(-4 , ios::cur);
         char tcp_data_buf[1024];
         memset(tcp_data_buf,0,sizeof(tcp_data_buf)/sizeof(char));
           if ( ( tempBuf[0]==0x50 && tempBuf[1]==0x4f && tempBuf[2]==0x53 && tempBuf[3]==0x54 ) ||
                        ( tempBuf[0]==0x47 && tempBuf[1]==0x45 && tempBuf[2]==0x54 )
                        ) //Two conditions are "POST" and "GET", judge the success shows that the network frame contains a HTTP get or post links
                    {
                        file->read(tcp_data_buf,m_dataSize);
                        m_packData = tcp_data_buf;
    
                    }
    
    
    
    }
}