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

	string filePath ="D:\\InputMaven\\RAW\\1_0\\NewPacket.pcap";
	CPcapManager *manager= new CPcapManager();
	
     manager->readPcapFile(filePath);
	 manager->parsePecapFile();
	

	
	getchar();
	return 0;
}
