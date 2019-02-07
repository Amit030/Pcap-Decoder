#include "stdafx.h"
#include "LayerTwoParser.h"

CLayerTwoParser ::CLayerTwoParser(){
    m_pktHeader= new packetHeader();
}

void CLayerTwoParser::performLevelTwoParsing(fstream *f,int* pos){
  try{
      f->read((char*)m_pktHeader,SIZE_OF_PACKET_HEADER);
    
  }
  catch (const istream::failure& e) {
    cout << "Exception opening/reading file";
  }
    *pos+= SIZE_OF_PACKET_HEADER + m_pktHeader->capture_len;
 
 
}

packetHeader * CLayerTwoParser::getPacketHeader(){
    return m_pktHeader;
}
void CLayerTwoParser::showLayerTwoData(){
    cout<<endl<<"-----------PACKET HEADER-------------"<<endl<<endl;
    cout<<"Orignal Length : "<<m_pktHeader->capture_len<<endl;
    cout<<"Length Captured : "<<m_pktHeader->capture_len<<endl;
    cout<<"Epoc Time : "<<m_pktHeader->timestamp_s<<endl;

}
