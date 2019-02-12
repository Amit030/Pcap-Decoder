#include "stdafx.h"
#include "UDP.h"


/**
* Function       :  CPcapManager :: CUDP
* Description    :  Default Constructor/ initializing the updHeader pointer 
*                   setting default value for member variables 
* Arguments      :  NULL
* Return-type    :  void 
*/
CUDP::CUDP(){
    m_pUdpHeader= new updHeader();
    m_dataSize=0;
    return;
}



/*
* Function       :  CUDP :: CUDP
* Description    :  Destrctor : delete the pointer 
* Arguments      :  None
* Return-type    :  None
*/
CUDP:: ~CUDP(){
    delete m_pUdpHeader;
    return;
}



/*
* Function       :  CUDP :: parseUdpHeader
* Description    :  parseUdpHeader
* Arguments      :  file pointer and total length
* Return-type    :  void 
*/

void CUDP ::parseUdpHeader(fstream * file,int totalLen){
    file->read((char*)m_pUdpHeader,sizeof(updHeader));
    m_dataSize = totalLen - file->tellg();
   
}


/*
* Function       :  CUDP :: getUdpHeader
* Description    :  return pointer to the strcture
* Arguments      :  void
* Return-type    : updHeader *
*/

updHeader* CUDP :: getUdpHeader(){
    return m_pUdpHeader; 
}


/*
* Function       :  CUDP :: getUpdData
* Description    :  return the data 
* Arguments      :  void
* Return-type    : string
*/
string CUDP :: getUpdData(){
    return m_packData;
}



/**
* Function       :  CUDP ::getDataSize
* Description    :  return the  data 
* Arguments      :  void
* Return-type    :  int
*/
int CUDP ::getDataSize(){ 
    return m_dataSize; 
}


/**
* Function       :  CUDP :: parseUdpData
* Description    :  read the data from file as per calculated data size
after writing  data into buffer  futher writes the data into a string varibale
* Arguments      :  file pointer 
* Return-type    :  void 
*/

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

        //commented set Two conditions are "POST" and "GET", judge the success shows that the network frame contains a HTTP get or post links

        /*
        if ( ( tempBuf[0]==0x50 && tempBuf[1]==0x4f && tempBuf[2]==0x53 && tempBuf[3]==0x54 ) ||
        ( tempBuf[0]==0x47 && tempBuf[1]==0x45 && tempBuf[2]==0x54 )
        ) 
        {
        file->read(udp_data_buf,m_dataSize);
        m_packData = udp_data_buf;

        }

        */
        file->read(udp_data_buf,m_dataSize);
        m_packData = udp_data_buf;
    }

}



