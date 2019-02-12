#include "stdafx.h"
#include "UDP.h"

CUDP::CUDP(){
    m_udpHeader= new updHeader();
    m_dataSize=0;
}
void CUDP ::parseUdpHeader(fstream * file,int totalLen){
    file->read((char*)m_udpHeader,sizeof(updHeader));
     m_dataSize = totalLen - file->tellg();
     if(m_dataSize != 0)
         cout<<"DATA in UDP "<<m_dataSize<<endl;

}
updHeader* CUDP :: getUdpHeader(){
    return m_udpHeader; 
}
string CUDP :: getUpdData(){
    return m_packData;
}
int CUDP ::getDataSize(){ 
   return m_dataSize; 
}
CUDP:: ~CUDP(){
    delete m_udpHeader;
}

void  CUDP :: parseUdpData(fstream *file){

    if(m_dataSize !=0){
        char tempBuf[4];
        m_packData="";
        std::string methodBuf;
        std::string l_urlBuf;
        std::string l_hostBuf;
        std::string l_uaBuf;
        file->read(tempBuf,4);
        file->seekg(-4 , ios::cur);
         char udp_data_buf[1024];
         memset(udp_data_buf,0,sizeof(udp_data_buf)/sizeof(char));
           if ( ( tempBuf[0]==0x50 && tempBuf[1]==0x4f && tempBuf[2]==0x53 && tempBuf[3]==0x54 ) ||
                        ( tempBuf[0]==0x47 && tempBuf[1]==0x45 && tempBuf[2]==0x54 )
                        ) //Two conditions are "POST" and "GET", judge the success shows that the network frame contains a HTTP get or post links
                    {
                        file->read(udp_data_buf,m_dataSize);
                        m_packData = udp_data_buf;
    
                    }
    
    
    
    }
}