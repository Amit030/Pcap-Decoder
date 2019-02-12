#include "stdafx.h"
#include "LayerTwoParser.h"



/**
* Function       :  CLayerTwoParser :: CLayerTwoParser
* Description    :  Default Constructor/ initializing the packetHeader pointer 
* Arguments      :  NULL
* Return-type    :  void 
*/

CLayerTwoParser ::CLayerTwoParser(){
    m_pPacketHeader= new packetHeader();
    return;
}



/**
* Function       :  CLayerTwoParser :: ~CLayerTwoParser
* Description    :  Destrucutor 
* Arguments      :  NULL
* Return-type    :  Node 
*/

CLayerTwoParser ::~ CLayerTwoParser(){
   delete m_pPacketHeader;
   return;
}

/**
* Function       :  CLayerTwoParser :: performLevelTwoParsing
* Description    :  parse the pcpap file into packetHeader strcture
* Arguments      :  file pointer , position pointer
* Return-type    :  void 
*/

void CLayerTwoParser::performLevelTwoParsing(fstream *f,int* pos){
  try{
      f->read((char*)m_pPacketHeader,SIZE_OF_PACKET_HEADER);
    
  }
  catch (const istream::failure& e) {
    cout << "Exception opening/reading file";
  }
    *pos+= SIZE_OF_PACKET_HEADER + m_pPacketHeader->capture_len;
 
}


/**
* Function       :  CLayerTwoParser :: getPacketHeader
* Description    :  return pointer to the strcture
* Arguments      :  Node
* Return-type    :  packetHeader *

*/
packetHeader * CLayerTwoParser::getPacketHeader(){
    return m_pPacketHeader;
}

/**
* Function       :  CLayerTwoParser :: showLayerTwoData
* Description    :  print the data of the strcuture
* Arguments      :  None
* Return-type    :  void 
*/
void CLayerTwoParser::showLayerTwoData(){
    cout<<endl<<"-----------PACKET HEADER-------------"<<endl<<endl;
    cout<<"Orignal Length : "<<m_pPacketHeader->capture_len<<endl;
    cout<<"Length Captured : "<<m_pPacketHeader->capture_len<<endl;
    cout<<"Epoc Time : "<<m_pPacketHeader->timestamp_s<<endl;

}
