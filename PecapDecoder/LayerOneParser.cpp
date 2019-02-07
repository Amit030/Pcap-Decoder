#include "stdafx.h"
#include "LayerOneParser.h"

CLayerOneParser ::CLayerOneParser(){
    gHeader= new globalHeader();
}

void CLayerOneParser::performLevelOneParsing(fstream *f, int* pos ){
    try{
        f->read((char*)gHeader,SIZE_OF_GLOBAL_HEADER); }
        catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

    *pos= SIZE_OF_GLOBAL_HEADER;
  

}

globalHeader * CLayerOneParser::getGlobalHeader(){
    return gHeader;
}
void CLayerOneParser::showLayerOneData(){
    cout<<endl<<"-----------GLOBAL HEADER-------------"<<endl<<endl;
    cout<<"Magic Number : "<<gHeader->magic<<endl;
    cout<<"Link Type : "<<gHeader->linktype<<endl;
    cout<<"EPOC Time : "<<gHeader->thiszone<<endl;



}
