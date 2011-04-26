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
	unsigned char ackNum[4];
	unsigned char dataOffset; // 4 bits
	unsigned char reserved; // 4 bits
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

struct tcpPacket *parseTCPPacket(struct ip4Packet *ipPacket)
{
	int i; int spacer = 0;
	struct tcpPacket *tcp = (struct tcpPacket *) malloc (sizeof(struct tcpPacket));
	
	for(i=0; i<2; i++)
	{
		tcp->sourcePort[i] = ipPacket->payload[i];
	}
	
	spacer = 2;
	for(i=0; i<2; i++)
	{
		tcp->destinationPort[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 4;
	for(i=0; i<4; i++)
	{
		tcp->seqNum[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 8;
	for(i=0; i<4; i++)
	{
		tcp->ackNum[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 12;
	tcp->dataOffset = ( ipPacket->payload[spacer] >> 4 );
	
	tcp->reserved = ( ipPacket->payload[spacer] << 4 );
	tcp->reserved = ( tcp->reserved >> 4 );
	
	spacer = 13;
	tcp->flags = ipPacket->payload[spacer];
	
	spacer = 14;
	for(i=0; i<2; i++)
	{
		tcp->windowSize[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 16;
	for(i=0; i<2; i++)
	{
		tcp->checksum[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 18;
	for(i=0; i<2; i++)
	{
		tcp->urgentPointer[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 20;
	// (tcp->dataOffset - 5) * 32 bits = size of options
	unsigned int optionLength = ( ( (tcp->dataOffset - 5) * 32 ) / 8 );
	for(i=0; i<optionLength ; i++)
	{
		tcp->options[i] = ipPacket->payload[ i + spacer ];
	}
	
	spacer = 20 + optionLength;
	tcp->size = 0;
	for(i=0; (i + spacer)<(ipPacket->size); i++)
	{
		tcp->payload[i] = ipPacket->payload[ i + spacer ];
		tcp->size++;
	}
	
	for(i=0; i<(ipPacket->size); i++)
	{
		tcp->rawPacket[i] = ipPacket->payload[i];
	}
	
	
	
	return tcp;
}

void printTCPPacket(struct tcpPacket *tcp)
{
	int i;
	/*
	unsigned char sourcePort[2];
	unsigned char destinationPort[2];
	unsigned char seqNum[4];
	unsigned char ackNUm[4];
	unsigned char dataOffset; // 4 bits
	unsigned char reserved; // 4 bits
	unsigned char flags;
	unsigned char windowSize[2];
	unsigned char checksum[2];
	unsigned char urgentPointer[2];
	unsigned char options[40];
	unsigned char padding[4];
	unsigned char payload[ (ETH_FRAME_LEN - 100) ];
	unsigned char rawPacket[ (ETH_FRAME_LEN - 35) ]; */
	
	printf("################################## TCP Packet ##################################\n");
	int convert = 0;
	printf("## Source Port: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", tcp->sourcePort[i]);
	}
	convert = tcp->sourcePort[0];
	printf(" (%d)", (convert << 8) + tcp->sourcePort[1]);
	printf(" | ");
	
	convert = 0;
	printf("Destination Port: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", tcp->destinationPort[i]);
	}
	convert = tcp->destinationPort[0];
	printf(" (%d)", (convert << 8) + tcp->destinationPort[1]);
	printf("\n");
	
	printf("## Sequence Number: ");
	for(i=0; i<4; i++)
	{
		printf("%02X", tcp->seqNum[i]);
	}
	printf(" | ");
	
	printf("Acknowledge Number: ");
	for(i=0; i<4; i++)
	{
		printf("%02X", tcp->ackNum[i]);
	}
	printf("\n");
	
	printf("## Data Offset: %X | ", tcp->dataOffset);
	
	printf("Reserved: %X | ", tcp->reserved);
	
	printf("Flags: %02X\n", tcp->flags);
	
	printf("## Window Size: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", tcp->windowSize[i]);
	}
	printf(" | ");
	
	printf("Checksum: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", tcp->checksum[i]);
	}
	printf(" | ");
	
	printf("Urgent Pointer: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", tcp->urgentPointer[i]);
	}
	printf("\n");
	
	unsigned int optionLength = ( ( (tcp->dataOffset - 5) * 32 ) / 8 );
	
	printf("## Options: ");
	for(i=0; i<optionLength; i++)
	{
		printf("%02X", tcp->options[i]);
	}
	printf("\n");
	
	if ( printPayload )
	{
		printf("## Payload:\n##  ");
		for(i=0; i<tcp->size-14; i++)
		{
			printf("%02X", tcp->payload[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}

	if ( printRaw )
	{
		printf("## Raw Packet:\n##  ");
		//size is the size of the payload
		//not entirely sure about the + and - but it works
		for(i=0; i<(tcp->size + optionLength + 6); i++)
		{
			printf("%02X", tcp->rawPacket[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}
	
	printf("################################################################################\n\n");
}



struct udpPacket
{
	unsigned char sourcePort[2];
	unsigned char destinationPort[2];
	unsigned char length[2];
	unsigned char checksum[2];
	unsigned char payload[ (ETH_FRAME_LEN - 35) ];
	unsigned char rawPacket[ (ETH_FRAME_LEN - 43) ];
	
	unsigned int size;
};

struct udpPacket *parseUDPPacket(struct ip4Packet *ip4Packet)
{
	struct udpPacket *udpPacket = (struct udpPacket *) malloc (sizeof(struct udpPacket));

	int i; int spacer=0; //spacer is the current octet

	for(i=0; i<2; i++)
	{
		udpPacket->sourcePort[i] = ip4Packet->payload[i+spacer];
		
	}
	spacer+=2;
	for(i=0; i<2; i++)
	{
		udpPacket->destinationPort[i] = ip4Packet->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<2; i++)
	{
		udpPacket->length[i] = ip4Packet->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<2; i++)
	{
		udpPacket->checksum[i] = ip4Packet->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<(ip4Packet->size-10); i++)
	{
		udpPacket->payload[i] = ip4Packet->payload[i+spacer];
	}
	spacer+=2;
	for(i=0; i<(ip4Packet->size-2); i++)
	{
		udpPacket->rawPacket[i] = ip4Packet->payload[i];
	}
	udpPacket->size = ip4Packet->size-12;

	return udpPacket;
}

void printUDPPacket(struct udpPacket *udpPacket)
{
	int i;
	printf("################################# UDP Packet ###################################\n");
	
	int convert = 0;
	printf("## Source Port: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", udpPacket->sourcePort[i]);
	}
	convert = udpPacket->sourcePort[0];
	printf(" (%d)", (convert << 8) + udpPacket->sourcePort[1]);
	printf(" | ");
	
	convert = 0;
	printf("Destination Port: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", udpPacket->destinationPort[i]);
	}
	convert = udpPacket->destinationPort[0];
	printf(" (%d)", (convert << 8) + udpPacket->destinationPort[1]);
	printf("\n");
	
	printf("## Length: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", udpPacket->length[i]);
	}
	printf(" | ");
	
	printf("Checksum: ");
	for(i=0; i<2; i++)
	{
		printf("%02X", udpPacket->checksum[i]);
	}
	printf("\n");
	
	if (printPayload)
	{
		printf("## Payload:\n##  ");
		for(i=0; i<(udpPacket->size-10); i++)
		{
			printf("%02X", udpPacket->payload[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}

	if (printRaw)
	{
		printf("## Raw Packet:\n##  ");
		for(i=0; i<(udpPacket->size-2); i++)
		{
			printf("%02X", udpPacket->rawPacket[i]);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}

	printf("################################################################################\n\n");

}
