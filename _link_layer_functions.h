/*
 *	Functions related to taking a raw hexidecimal string of ethernet frame data and
 *	filling in the values for an ethernet struct are defined here. The values include all fields specified
 *	in the 802.3 headers. The raw hex stream is saved in the struct as well for printing and
 *	the payload is saved to be passed to other functions to parse IPv4 and IPv6. The ethernet
 *	FCS is being calculated for timing and purely academic purposes. There are also functions
 *	to print the values of the ethernet struct, as this is the prupose of the application.
 *
 *	File: 	_link_layer_functions.h
 *	Author:	Michael McAtee
 */

/* Defined values for Ethertype field specified in 802.3 */
unsigned char arpType[2] = {0x08, 0x06}; /* ARP  */
unsigned char ip4Type[2] = {0x08, 0x00}; /* IPv4  */
unsigned char ip6Type[2] = {0x86, 0xDD}; /* IPv6  */

/* C struct to hold a full ethernet packet */
struct ethernetFrame
{
	unsigned char destAddr[6];						/* Destination MAC address (6 octets) */
	unsigned char sourceAddr[6];					/* Source MAC address (6 octets) */
	unsigned char etherType[2];						/* Ethertype used to define frame payload */
	unsigned int  FCS;								/* Frame Check Sequence (calculated in this framework) */
	double 		  fcsBenchmark;						/* Time in seconds it took to calculate the FCS */
	unsigned int  size;								/* Size of the ethernet frame as specified by the recv function in socket programming */
	unsigned char payload[ (ETH_FRAME_LEN - 14) ];	/* Payload from the ethernet frame to be passed to higher layer header files */
	unsigned char rawFrame[ ETH_FRAME_LEN ];		/* The raw frame data used to populate the struct */
	struct ip4Packet *ip4payload;					/* Pointer to the IPv4 packet data stored in a custom struct */
};

/*
 *	Functions
 */

int createSocket()
{
	return socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); /* Open a RAW socket with Ethernet Frame included */
}

/* Using socket programming grab the raw ethernet frame off the wire and place it in a buffer, while returning the size */
unsigned int getLinkLayerFrame(int eth_socket, unsigned char* buffer)
{
	if (eth_socket == -1) { printf("ERROR, Socket did not bind for some reason."); error(1); } /* Error in binding socket */
	return recv(eth_socket, buffer, ETH_FRAME_LEN, 0); /* Return the size of the frame passed into the buffer */
}

/* Using a buffer and size input to output an ethernet struct */
struct ethernetFrame *parseFrame(unsigned char* buffer, unsigned int SIZE)
{
	/* Create an ethernet struct and allocate space for it in memory */
	struct ethernetFrame *returnFrame = (struct ethernetFrame *) malloc (sizeof(struct ethernetFrame));
	
	int i; int spacer;
	
	/* Size returned from grabbing the frame from the socket in recv() */
	returnFrame->size = SIZE;
	
	/* Populate the destination MAC address one octet at a time */
	for(i=0; i<6; i++)
	{
		returnFrame->destAddr[i] = buffer[i];
	}
	spacer = 6;
	
	/* Populate the source MAC address one octet at a time */	
	for(i=0; i<6; i++)
	{
		returnFrame->sourceAddr[i] = buffer[spacer + i];
	}
	spacer = 12;
	
	/* Populate the ethertype one octet at a time */
	for(i=0; i<2; i++)
	{
		returnFrame->etherType[i] = buffer[spacer + i];
	}
	spacer = 14;
	
	/* Populate the payload one octet at a time */
	for(i=0; i<(SIZE - spacer); i++)
	{
		returnFrame->payload[i] = buffer[spacer + i];
	}
	
	/* Populate the raw full frame one octet at a time */
	for(i=0; i < ETH_FRAME_LEN; i++)
	{
		returnFrame->rawFrame[i] = buffer[i];
	}
	
	startTimer = startTimeBenchmark(); /* Start timer for CRC calculation */
	/* Populate the FCS of the struct after caculating the CRC for the frame */
	returnFrame->FCS = crc32(buffer, SIZE);
	returnFrame->fcsBenchmark = getTimeToRun(startTimer); /* Stop the timer for CRC calculation and set struct value */
	
	if ( checkEtherType(returnFrame, ip4Type) && checkIPPacketType(returnFrame) == 4 )
	{
		returnFrame->ip4payload = parseIP4Packet(returnFrame);
	}
	
	/* return the newly populated struct */
	return returnFrame;
}

/* Print a populated struct to stdout */
void printFrame(struct ethernetFrame *frame)
{
	int i; unsigned char temp;
	
	printf("\n################################ Ethernet Frame ################################\n");
	
	printf("## Destination Address: ");
	for(i=0; i<6; i++)
	{
		printf("%X", (frame->destAddr[i] >> 4));	/* Print the first and second 4 bits of the octet */
		temp = (frame->destAddr[i] << 4); temp >>= 4;
		printf("%X", temp);
		if ( i != 5 ) { printf(":"); }				/* Print colon between octet values */
	}
	
	printf("\n## Source Address: ");
	for(i=0; i<6; i++)
	{
		printf("%X", (frame->sourceAddr[i] >> 4));	/* Print the first and second 4 bits of the octet */
		temp = (frame->sourceAddr[i] << 4); temp >>= 4;
		printf("%X", temp);
		if ( i != 5 ) { printf(":"); }				/* Print colon between octet values */
	}
	
	/* Print the ethertype one octect after another */
	printf("\n## EtherType: ");
	for(i=0; i<2; i++)
	{
		printf("%X", (frame->etherType[i] >> 4));
		temp = (frame->etherType[i] << 4) >> 4;
		printf("%X", temp);
	}
	/* Print ethertype description for IPv4 */
	if( checkEtherType(frame, ip4Type) ) {printf(" - DOD Internet Protocol (IP)");}
	/* Print ethertype description for IPv6 */
	if( checkEtherType(frame, ip6Type) ) {printf(" - IP version 6");}
	/* Print ethertype description for ARP */
	if( checkEtherType(frame, arpType) ) {printf(" - Address Resolution Protocol (ARP)");}
	printf("\n");
	
	printf("## CRC Value: %X%X%X%X%X%X%X%X (%f seconds)\n",
		(frame->FCS >> 28), ((frame->FCS << 4) >> 28), ((frame->FCS << 8) >> 28), 
		((frame->FCS << 12) >> 28), ((frame->FCS << 16) >> 28), ((frame->FCS << 20) >> 28),
		((frame->FCS << 24) >> 28), ((frame->FCS << 28) >> 28), (frame->fcsBenchmark)
	);
	
	if ( printPayload )
	{
		printf("## Payload:\n##  ");
		for(i=0; i<(frame->size - 14); i++)
		{ 
			printf("%X", (frame->payload[i] >> 4));
			temp = frame->payload[i] << 4; temp >>= 4;
			printf("%X", temp);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}
	
	if ( printRaw )
	{
		printf("## Raw Frame:\n##  ");
		for(i=0; i<frame->size; i++)
		{
			printf("%X", (frame->rawFrame[i] >> 4));
			temp = frame->rawFrame[i] << 4; temp >>= 4;
			printf("%X", temp);
			if( ((i+1)%36) == 0 && i != 0 ) { printf("\n##  "); }
		}
		printf("\n");
	}
	printf("################################################################################\n\n");
	
	if ( frame->ip4payload != NULL )
	{
		printIP4Packet(frame->ip4payload);
	}
}

/* function to return a 1 if the ethertype matches the ethertype array passed to it and 0 if it doesn't */
int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray)
{
	if( frame->etherType[0] == etherTypeArray[0]  && frame->etherType[1] == etherTypeArray[1] ) { return 1; }
	else { return 0; }
}

/*
 *	CRC Calculations
 *	Code taken from http://www.faqs.org/faqs/compression-faq/part1/section-26.html
 */
	
	/* Initialized first time "crc32()" is called. If you prefer, you can statically initialize it at compile time. */
	unsigned int crc32_table[256];

	unsigned int crc32(unsigned char *buf, unsigned int len)
	{
		    unsigned char 	*p;
		    unsigned int 	crc;

		    if ( !crc32_table[1] )  /* if not already done, */
		    {
		            init_crc32();   /* build table */
		    }
		    
		    crc = 0xffffffff;       /* preload shift register, per CRC-32 spec */
		    
		    for (p = buf; len > 0; ++p, --len)
		    {
		            crc = (crc << 8) ^ crc32_table[(crc >> 24) ^ *p];
		    }
		    
		    return ~crc;            /* transmit complement, per CRC-32 spec */
	}

	/* Build auxiliary table for parallel byte-at-a-time CRC-32. */
	#define CRC32_POLY 0x04c11db7 /* AUTODIN II, Ethernet, & FDDI */

	void init_crc32()
	{
		    int i, j;
		    unsigned int c;

		    for (i = 0; i < 256; ++i)
		    {
		            for (c = i << 24, j = 8; j > 0; --j)
		            {
		                    c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
		            }
		            
		            crc32_table[i] = c;
		    }
	}
