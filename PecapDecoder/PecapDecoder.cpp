// PecapDecoder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Structures.h"
#include "PecapManager.h"
#include <string>
#include <Winsock.h>




typedef signed int gint32;
typedef unsigned short u_short;
typedef unsigned int guint32;
typedef unsigned char u_char;
using namespace std;

int _tmain(int argc, _TCHAR* argv[]){

	string filePath ="D:\\ISC\\RAW";
	CPcapManager *manager= new CPcapManager();
    cout<<"------------------------------------------------------------\n";
    cout<<"-------------------------PECAP DECODER----------------------\n";
    cout<<"------------------------------------------------------------\n\n\n";
   
    string input,output;
    cout<<"Please Enter the path of Source directory"<<endl;
    getline(cin,input);

     cout<<"Please Enter the path of Output directory"<<endl;
    getline(cin,output);
    
	
     manager->readPcapFile(input,output);
	// manager->parsePecapFile();
	getchar();
	return 0;
}
