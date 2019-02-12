#include "stdafx.h"
#include "InternetVersion.h"



/**
* Function       :  CInternetVersion :: CInternetVersion
* Description    :  Default Constructor/ initializing the ipheader pointer 
* Arguments      :  NULL
* Return-type    :  void 
*/
CInternetVersion :: CInternetVersion(){
    m_pIPV4Header= new ipv4Header();
}



/**
* Function       :  CInternetVersion :: ~CInternetVersion
* Description    :  Destrucutor 
* Arguments      :  NULL
* Return-type    :  Node 
*/
CInternetVersion :: ~CInternetVersion(){
    delete m_pIPV4Header;
}



/**
* Function       :  CInternetVersion :: parseIPV4Header
* Description    :  parse the pcpap file into m_pIPV4Header strcuture 
*                   till the size of IpHeader 
* Arguments      :  file pointer
* Return-type    :  void 
*/
void CInternetVersion::parseIPV4Header(fstream *file){
    try{ file->read((char*)m_pIPV4Header,sizeof(ipv4Header));

    }
    catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

}

/**
* Function       :  CInternetVersion :: getIPV4header
* Description    :  return pointer to the strcture
* Arguments      :  void
* Return-type    :  ipv4Header *
*/
ipv4Header *  CInternetVersion:: getIPV4header(){
    return m_pIPV4Header;
}



/**
* Function       :  CInternetVersion :: showInternetVersionData
* Description    :  print the data of the strcuture
* Arguments      :  void
* Return-type    :  void 
*/
void CInternetVersion:: showInternetVersionData(){
    cout<<endl<<"-----------IP HEADER-------------"<<endl<<endl;
    cout<<"Destination IP address : "<<inet_ntoa(m_pIPV4Header->ip_dst)<<endl;
    cout<<"Source IP address : "<<inet_ntoa(m_pIPV4Header->ip_src)<<endl;
    cout<<"IPV4 Total Length  : "<<ntohs(m_pIPV4Header->TotalLen)<<endl;
}