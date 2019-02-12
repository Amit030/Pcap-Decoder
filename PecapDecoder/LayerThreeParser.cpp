#include "stdafx.h"
#include "LayerThreeParser.h"



/**
* Function       :  CPcapManager :: CLayerThreeParser
* Description    :  Default Constructor/ initializing the etherHeader pointer to NULL
                     initializing the linuxHeader pointer to NULL
* Arguments      :  NULL
* Return-type    :  void 
*/

CLayerThreeParser ::CLayerThreeParser(){
    m_pEtherHeader= NULL;
    linuxHeader=NULL;
    return;
}

/**
* Function       : CLayerThreeParser :: ~CLayerThreeParser
* Description    :  Destrucutor 
* Arguments      :  NULL
* Return-type    :  Node 
*/

CLayerThreeParser :: ~CLayerThreeParser(){
   delete m_pEtherHeader;
   return;
}


/**
* Function       :  CPcapManager :: performLevelThreeParsing
* Description    :  parse the pcpap file into etherHeader strcuture if linkType== ETHERNET_CAPTURES
                    till the size of IpHeader 
* Arguments      :  file pointer , position pointer, linktype
* Return-type    :  void 
*/

void CLayerThreeParser::performLevelThreeParsing(fstream *f, u_int linkType,int* pos){
    if(linkType== ETHERNET_CAPTURES){
        m_pEtherHeader= new etherHeader();
        try{
            f->read((char*)m_pEtherHeader,SIZE_OF_EATHER_HEADER); }
        catch (const istream::failure& e) {
            cout << "Exception opening/reading file";
        }
    }
    return;
}


/**
* Function       :  CPcapManager :: getEtherHeader
* Description    :  return pointer to the strcture
* Arguments      :  None
* Return-type    :  etherHeader * 
*/

etherHeader * CLayerThreeParser::getEtherHeader(){
    return m_pEtherHeader;
}



/**
* Function       :  CPcapManager :: showLayerThreeData
* Description    :  print the data of the strcuture
* Arguments      :  None
* Return-type    :  void 
*/

void CLayerThreeParser::showLayerThreeData(){
    cout<<endl<<"-----------ETHERNET HEADER-------------"<<endl<<endl;
    cout<<"Source MAC Address : "<<m_pEtherHeader->SrcMAC<<endl;
     cout<<"Destination MAC Address : "<<m_pEtherHeader->DstMAC<<endl;
    cout<<"Frame type : "<<m_pEtherHeader->FrameType<<endl;
}