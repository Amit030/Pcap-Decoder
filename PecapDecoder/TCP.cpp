#include "stdafx.h"
#include "TCP.h"



/**
* Function       :  CTCP :: CUDP
* Description    :  Default Constructor/ initializing the tcpHeader pointer;
*                   setting default value for member variables 
* Arguments      :  NULL
* Return-type    :  void 
*/
CTCP::CTCP(){
    m_pTcpHeader= new tcpHeader();
    m_dataSize=0;
}


/*
* Function       :  CTCP :: CTCP
* Description    :  Destrctor : delete the pointer 
* Arguments      :  None
* Return-type    :  None
*/
CTCP::~ CTCP(){
    delete m_pTcpHeader;
}


/**
* Function       :  CTCP :: parseTcpHeader
* Description    :  parse the pcpap file into tcpHeader structure
*                   till the size of IpHeader 
* Arguments      :  file pointer , position pointer, linktype
* Return-type    :  void 
*/
void CTCP::parseTcpHeader(fstream *file,int totalLen){
    try{
        file->read((char*)m_pTcpHeader,sizeof(tcpHeader)); }
    catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

    m_dataSize = totalLen - file->tellg();

    file->seekg((m_pTcpHeader->HeaderLen >> 2)-sizeof(tcpHeader),ios::cur);
    parseTcpData(file);
}




/**
* Function       :  CTCP :: getTcpHeader
* Description    :  return pointer to the strcture
* Arguments      :  void
* Return-type    : tcpHeader *
*/

tcpHeader * CTCP:: getTcpHeader(){
    return m_pTcpHeader;
}



/**
* Function       :  CTCP :: getDataSize
* Description    :  return the size of data 
* Arguments      :  void
* Return-type    :  int 
*/
int  CTCP:: getDataSize(){
    return m_dataSize;
}



/**
* Function       :  CTCP :: getTcpData
* Description    :  return the  data 
* Arguments      :  void
* Return-type    :  string 
*/
string CTCP:: getTcpData(){
 return m_packData;
}


/**
* Function       :  CTCP :: showTcpData
* Description    :  print the data of the strcuture
* Arguments      :  void
* Return-type    :  void 
*/
void CTCP:: showTcpData(){


    char temp = m_pTcpHeader->HeaderLen;
    int headerLen = 0xF0 & temp;
    headerLen = headerLen >> 4;
    cout<<endl<<"-----------TCP HEADER-------------"<<endl<<endl;
    cout<<"Src port  : "<<ntohs(m_pTcpHeader->SrcPort)<<endl;
    cout<<"Dest port : "<<ntohs(m_pTcpHeader->DstPort)<<endl;
    cout<<"Acknowledge Number : "<<m_pTcpHeader->AckNO<<endl;
    cout<<"Header Len  : "<< headerLen*4<<endl;
    cout<<"Window  : "<<ntohs(m_pTcpHeader->Window)<<endl;
    cout<<"Checksum  : "<<m_pTcpHeader->Checksum<<endl;
    cout<<"UrgentPointer : "<<m_pTcpHeader->UrgentPointer<<endl;

    cout<<"______________________________TCP FLAG________________________________"<<endl;
    switch(m_pTcpHeader->th_flags){
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


/**
* Function       :  CTCP :: parseTcpData
* Description    :  read the data from file as per calculated data size 
                    after writing  data into buffer  futher writes the data into a string varibale
* Arguments      :  file pointer 
* Return-type    :  void 
*/
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
        char tcp_data_buf[2000];
        memset(tcp_data_buf,0,sizeof(tcp_data_buf)/sizeof(char));

        
       // this statement will print the request only


        /*   if ( ( tempBuf[0]==0x50 && tempBuf[1]==0x4f && tempBuf[2]==0x53 && tempBuf[3]==0x54 ) ||
            ( tempBuf[0]==0x47 && tempBuf[1]==0x45 && tempBuf[2]==0x54 )
            ) //Two conditions are "POST" and "GET", judge the success shows that the network frame contains a HTTP get or post links
        {
            file->read(tcp_data_buf,m_dataSize);
            m_packData = tcp_data_buf;
        }
        */

        file->read(tcp_data_buf,m_dataSize);
            m_packData = tcp_data_buf;
    }
}




