 /*
 *	
 *
 *	File: 	_network_layer_functions.h
 *	Author:	Michael McAtee
 */

/* Defined values for Protocol field specified in 802.3?probably */
unsigned char tcpType  = {0x06}; /* tcp  */
unsigned char udpType  = {0x11}; /* udp  */
unsigned char icmpType = {0x01}; /* icmp */

/* C struct to hold IPv4 packet information */
struct ip4Packet
{
	unsigned char ipVersion;
	unsigned char headerLength;
	unsigned char dscpECN;
	unsigned char totalLength[2];
	unsigned char identification[2];
	unsigned char flagsNOffset[2];
	unsigned char TTL;
	unsigned char protocol;
	unsigned char headerChecksum[2];
	unsigned char sourceAddress[4];
	unsigned char destinationAddress[4];
	unsigned char options[4];
	unsigned char payload[ (ETH_FRAME_LEN - 35) ];
	unsigned char rawPacket[ (ETH_FRAME_LEN - 14) ];

	unsigned int size;
	struct tcpPacket *tcp;
	struct udpPacket *udp;
};


/* Functions */

struct ip4Packet *parseIP4Packet(struct ethernetFrame *linkFrame)
{
	struct ip4Packet *packet = (struct ip4Packet *) malloc (sizeof(struct ip4Packet));
	
	int i; int spacer=0; //spacer is the current octet
	packet->ipVersion = (linkFrame->payload[spacer] >> 4);
	packet->headerLength = (linkFrame->payload[spacer] << 4);
	packet->headerLength = (packet->headerLength >> 4);
	spacer = 1;
	packet->dscpECN = linkFrame->payload[spacer];
	spacer++;
	for(i=0; i<2; i++)
	{
		packet->totalLength[i] = linkFrame->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<2; i++)
	{
		packet->identification[i] = linkFrame->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<2; i++)
	{
		packet->flagsNOffset[i] = linkFrame->payload[i+spacer];
	}
	spacer+=2;
	packet->TTL= linkFrame->payload[spacer];
	spacer++;
	packet->protocol = linkFrame->payload[spacer];
	spacer++;
	for(i=0; i<2; i++)
	{
		packet->headerChecksum[i] = linkFrame->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<4; i++)
	{
		packet->sourceAddress[i] = linkFrame->payload[i+spacer];
	}
	spacer+=4;
	for(i=0; i<4; i++)
	{
		packet->destinationAddress[i] = linkFrame->payload[i+spacer];
	}
	spacer+=4;
	
	if (packet->headerLength == 6)
	{
		for(i=0; i<4; i++)
		{
			packet->options[i] = linkFrame->payload[i+spacer];
		}
		spacer+=4;
	}
	
	for(i=0; i<(linkFrame->size-20); i++)
	{
		packet->payload[i] = linkFrame->payload[i+spacer];
	}
	
	packet->size = linkFrame->size - spacer;
	
	for(i=0; i<(linkFrame->size-14); i++)
	{
		packet->rawPacket[i] = linkFrame->payload[i];
	}
	
	if( checkProtocolType(packet, tcpType) )
	{
		packet->tcp = parseTCPPacket(packet);
	}
	
	if( checkProtocolType(packet, udpType) )
	{
		packet->udp = parseUDPPacket(packet);
	}

	return packet;
}

void printIP4Packet(struct ip4Packet *packet)
{
	int i;
	
	printf("################################# IPv4 Packet ##################################\n");
	
	printf("## IP Version: %X | ", packet->ipVersion);
	
	printf("Header Length: %X | ", packet->headerLength);
	
	printf("DSCPECN: %02X  | ", packet->dscpECN);
	
	printf("Total Length: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", packet->totalLength[i]);
	}
	printf("\n");
	
	printf("## Identification: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", packet->identification[i]);
	}
	printf(" | ");
	
	printf("Flags and Offset: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", packet->flagsNOffset[i]);
	}
	printf(" | ");
	
	printf("TTL: %02X \n", packet->TTL);
	
	printf("## Protocol Number: %02X ", packet->protocol);
	if (checkProtocolType(packet, tcpType)) {printf(" - Transmission Control Protocol (TCP)");}
	if (checkProtocolType(packet, udpType)) {printf(" - User Datagram Protocol (UDP)");}
	if (checkProtocolType(packet, icmpType)) {printf(" - Internet Control Message Protocol (ICMP)");}
	printf("\n");


	printf("## Header Checksum: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", packet->headerChecksum[i]);
	}
	printf("\n");
	
	printf("## Source IP Address: ");
	for(i=0; i<4; i++)
	{
		printf("%d", packet->sourceAddress[i]);
		if( i != 3 ) { printf("."); }
	}
	printf(" (");
	for(i=0; i<4; i++)
	{
		printf("%02X", packet->sourceAddress[i]);
	}
	printf(")\n");
	
	printf("## Destination IP Address: ");
	for(i=0; i<4; i++)
	{
		printf("%d", packet->destinationAddress[i]);
		if( i != 3 ) { printf("."); }
	}
	printf(" (");
	for(i=0; i<4; i++)
	{
		printf("%02X", packet->destinationAddress[i]);
	}
	printf(")\n");
	
	if ( printPayload )
	{
		printf("## Payload:\n##  ");
		for(i=0; i<(packet->size-14); i++)
		{
			printf("%02X", packet->payload[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}

	if ( printRaw )
	{
		printf("## Raw Packet:\n##  ");
		//size is the size of the payload
		//not entirely sure about the + and - but it works
		for(i=0; i<(packet->size+6); i++)
		{
			printf("%02X", packet->rawPacket[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}
	printf("################################################################################\n\n");
	
	if( packet->tcp != NULL )
	{
		printTCPPacket(packet->tcp);
	} else if ( packet->udp != NULL )
	{
		printUDPPacket(packet->udp);
	}
}

int checkIPPacketType(struct ethernetFrame *linkFrame)
{
	unsigned short int version = ( linkFrame->payload[0] >> 4 );
	return version;
}

int checkProtocolType(struct ip4Packet *packet, unsigned char protocolType)
{
	if (packet == NULL) { return 0; }
	if(packet->protocol == protocolType ) { return 1; }
	else { return 0; }
}

