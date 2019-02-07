#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "stdafx.h"
#include <Winsock.h>
#include <iostream>
#include <fstream>
using namespace std;


typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;

#define SIZE_OF_GLOBAL_HEADER       (24)
#define SIZE_OF_PACKET_HEADER       (16)
#define SIZE_OF_EATHER_HEADER       (14)
#define SIZE_OF_IP_HEADER           (20)





/*
Pcap file 24B each field description: 
Magic: 4B: 0x1A 2B 3C 4D: used to mark files
Major: 2B, 0x02 00: version number for the current file key
Minor: 2B, The current file version number 0x04 00 secondary
ThisZone: Standard time 4B local; all zero
SigFigs: 4B timestamp accuracy; all zero
SnapLen: The largest storage length 4B
LinkType: 4B link type
Common types: 
　0 BSD loopback devices, except for later OpenBSD
 1 Ethernet, and Linux loopback devices
 6 802.5 Token Ring
 7 ARCnet
 8 SLIP
 9 PPP
 */
typedef struct globalHeader {
    u_int magic;
    u_short version_major;
    u_short version_minor;
    int thiszone;
    u_int sigfigs;
    u_int snaplen;
    u_int linktype;
}pcap_header;

/*
The composition of Packet Baotou and Packet data
The field description: 
Timestamp: Time stamp is high, accurate to seconds
Timestamp: Timestamp low, accurate to microseconds
Caplen: The current data region length, i.e. the data frame length to grab, we can get the next data frame position. 
Len: Off-line data length: the actual data frames in the network length, generally not more than caplen, in most cases, and Caplen value. 
Packet data: Packet (usually is the link layer data frame) contents, Length is Caplen, The length of the, Is the next Packet data stored in the current PCAP file, That is to say: PCAP file was not specified what interval between string of Packet data packet capture, The starting position the next group of data in the file. We need to rely on the first Packet packets to determine. 
*/

typedef struct timestamp{
    u_int timestamp_s;
    u_int timestamp_ms;
}timestamp;

typedef struct packetHeader{
    u_int timestamp_s;
    u_int timestamp_ms;
    u_int capture_len;
    u_int len;

}pkt_header;


typedef struct etherHeader
{
    u_char DstMAC[6]; //The destination MAC address
    u_char SrcMAC[6]; //The source MAC address
    u_short FrameType; //Frame type
} Ethernet;


typedef struct Linux_cooked_capture
{
    u_short package_type;
    u_short address_type;
    u_short address_length;
    u_short un_used[4];
    u_short FrameType; //Frame type
};




struct ipv4Header {
    u_char Ver_HLen; //Version + header length
    u_char TOS; //Type of service
    u_short TotalLen; //Total length
    u_short ID; //Identification
    u_short ip_off;		/* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* dont fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char TTL; //Life cycle
    u_char Protocol; //Protocol type
    u_short Checksum; //Header checksum
    in_addr ip_src;//The Source IP address
    in_addr ip_dst; //The destination IP addres
};

typedef u_int tcp_seq;
typedef struct tcpHeader
{ //TCP header
    u_short SrcPort;	/* source port */
    u_short DstPort;	/* destination port */
    tcp_seq SeqNO;		/* sequence number */
    tcp_seq AckNO;		/* acknowledgement number */
    u_char HeaderLen;	/* data offset, rsvd */
#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_SYN_ACK 0x12

#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short Window;		/* window */
    u_short Checksum; //The checksum
    u_short UrgentPointer; //Urgent pointer
};

typedef struct updHeader
{
    u_short source_port;//Source port
    u_short dest_port;//Destination port
    u_char placeholder;
    u_short udp_length;
};
typedef struct sessionInfo {
        string clientIP;
        string serverIP;
        string clientPort;
        string serverPort;
        int Rx;
        int Tx;
        int packetCount;
        string transportProtocol;   
        sessionInfo(string cIP,string sIP,string sPort,string cPort,int x, int y,string protocol){
            clientIP=cIP;
            serverIP=sIP;
            clientPort=cPort;
            serverPort=sPort;
            Rx=x;
            Tx=y;
            packetCount=1;
            transportProtocol =protocol;		
        }
    } m_session;

#endif
