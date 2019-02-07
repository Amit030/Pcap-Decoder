#include "stdafx.h"
#include "UDP.h"

CUDP::CUDP(){
    m_udpHeader= new updHeader();
    m_dataSize=0;
}
void CUDP ::parseUdpHeader(fstream * file,int totalLen){
    file->read((char*)m_udpHeader,sizeof(updHeader));
     m_dataSize = totalLen - file->tellg();

}
updHeader* CUDP :: getUdpHeader(){
    return m_udpHeader; 
}
void CUDP :: showUpdData(){
    
}
int CUDP ::getDataSize(){ 
   return m_dataSize; 
}
CUDP:: ~CUDP(){
    delete m_udpHeader;
}