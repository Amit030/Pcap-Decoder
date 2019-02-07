#include "stdafx.h"
#include "LayerThreeParser.h"

CLayerThreeParser ::CLayerThreeParser(){
    m_ethHeader= NULL;
    linuxHeader=NULL;
}

void CLayerThreeParser::performLevelThreeParsing(fstream *f, u_int linkType,int* pos){
    if(linkType== ETHERNET_CAPTURES){
        m_ethHeader= new etherHeader();
        try{
            f->read((char*)m_ethHeader,SIZE_OF_EATHER_HEADER); }
        catch (const istream::failure& e) {
            cout << "Exception opening/reading file";
        }


    }

    return;

}

etherHeader * CLayerThreeParser::getEtherHeader(){
    return m_ethHeader;
}

void CLayerThreeParser::showLayerThreeData(){
    cout<<endl<<"-----------ETHERNET HEADER-------------"<<endl<<endl;
    //cout<<"Source MAC Address : "<<inet_ntoa(m_ethHeader->SrcMAC)<<endl;
    // cout<<"Destination MAC Address : "<<inet_ntoa(m_ethHeader->DstMAC)<<endl;
    cout<<"Frame type : "<<m_ethHeader->FrameType<<endl;
}