#include "stdafx.h"
#include "InternetVersion.h"

CInternetVersion :: CInternetVersion(){
    ipHeader= new ipv4Header();
}

void CInternetVersion::parseIPV4Header(fstream *file){
    try{ file->read((char*)ipHeader,sizeof(ipv4Header));
  
    }
    catch (const istream::failure& e) {
        cout << "Exception opening/reading file";
    }

}
ipv4Header *  CInternetVersion:: getIPV4header(){
    return ipHeader;
}
void CInternetVersion:: showInternetVersionData(){
    cout<<endl<<"-----------IP HEADER-------------"<<endl<<endl;
    cout<<"Destination IP address : "<<inet_ntoa(ipHeader->ip_dst)<<endl;
    cout<<"Source IP address : "<<inet_ntoa(ipHeader->ip_src)<<endl;
    cout<<"IPV4 Total Length  : "<<ntohs(ipHeader->TotalLen)<<endl;
}