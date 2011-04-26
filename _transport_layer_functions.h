 /*
 *	
 *
 *	File: 	_transport_layer_functions.h
 *	Author:	Michael McAtee
 */


struct tcpPacket
{
	unsigned char sourcePort[2];
	unsigned char destinationPort[2];
	unsigned char seqNum[4];
	unsigned char ackNUm[4];
	unsigned char dataOffset;
	unsigned char reserved;
	unsigned char flags;
	unsigned char windowSize[2];
	unsigned char checksum[2];
	unsigned char urgentPointer[2];
	unsigned char options[40];
	unsigned char padding[4];
	unsigned char payload[ (ETH_FRAME_LEN - 100) ];
	unsigned char rawPacket[ (ETH_FRAME_LEN - 35) ];
	
	unsigned int size;
};
