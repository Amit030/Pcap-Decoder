#include "stdafx.h"
#include "LayerOneParser.h"



/**
* Function       :  CLayerOneParser :: CLayerOneParser
* Description    :  Default Constructor/ initializing the global header pointer 
* Arguments      :  NULL
* Return-type    :  void 
*/

CLayerOneParser ::CLayerOneParser(){
    m_pGlobalHeader= new globalHeader();
}

/**
* Function       : CLayerOneParser ::CLayerOneParser
* Description    :  Destrucutor 
* Arguments      :  NULL
* Return-type    :  Node 
*/

CLayerOneParser :: ~CLayerOneParser(){
   delete m_pGlobalHeader;
}

/**
* Function       :  CLayerOneParser :: performLevelOneParsing
* Description    :  parse the pcpap file into m_pIPV4Header strcuture 
                    till the size of IpHeader 
* Arguments      :  file pointer , position pointer
* Return-type    :  void 
*/
void CLayerOneParser::performLevelOneParsing(fstream *f, int* pos ){
    try{
        f->read((char*)m_pGlobalHeader,SIZE_OF_GLOBAL_HEADER); }
        catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

    *pos= SIZE_OF_GLOBAL_HEADER;
  

}


/*
* Function       :  CLayerOneParser ::getGlobalHeader
* Description    :  return pointer to the strcture
* Arguments      :  void
* Return-type    :  globalHeader *
*/
globalHeader * CLayerOneParser::getGlobalHeader(){
    return m_pGlobalHeader;
}



/**
* Function       :  CLayerOneParser :: showLayerOneData
* Description    :  print the data of the strcuture
* Arguments      :  void
* Return-type    :  void 
*/
void CLayerOneParser::showLayerOneData(){
    cout<<endl<<"-----------GLOBAL HEADER-------------"<<endl<<endl;
    cout<<"Magic Number : "<<m_pGlobalHeader->magic<<endl;
    cout<<"Link Type : "<<m_pGlobalHeader->linktype<<endl;
    cout<<"EPOC Time : "<<m_pGlobalHeader->thiszone<<endl;



}
