/*
 *	Functions related to getting link layer data as well as printing link layer raw hexidecimal values
 *
 *	File: _link_layer_functions.c
 */

unsigned int getLinkLayerFrame(unsigned char* buffer);
struct ethernetFrame *parseFrame(unsigned char* buffer, unsigned int SIZE);
void printFrame(struct ethernetFrame *frame);
int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray);
void descEtherType(struct ethernetFrame *frame);

unsigned int crc32(unsigned char *buf, unsigned int len);
void init_crc32();

unsigned int crcCalc(unsigned char *buffer, unsigned int SIZE);


unsigned char ARPetherType[2] = {0x08, 0x06}; /* ARP  */
unsigned char IP4etherType[2] = {0x08, 0x00}; /* IPv4  */
unsigned char IP6etherType[2] = {0x86, 0xDD}; /* IPv6  */

struct ethernetFrame
{
	unsigned char	destAddr[6];
	unsigned char	sourceAddr[6];
	unsigned char	etherType[2];
	unsigned int	FCS;
	double 			fcsBenchmark;
	unsigned int	size;
	
	unsigned char	payload[(ETH_FRAME_LEN - 14)];	
	unsigned char	rawFrame[ETH_FRAME_LEN];
};




unsigned int getLinkLayerFrame(unsigned char* buffer)
{
	int eth_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); /* Open a RAW socket with Ethernet Frame included */
	if (eth_socket == -1) { printf("ERROR, Socket did not bind for some reason."); error(1); } /* Error in binding socket */
	
	return recv(eth_socket, buffer, ETH_FRAME_LEN, 0); 		/* Read socket into buffer array while recv is not in error */
}

struct ethernetFrame *parseFrame(unsigned char* buffer, unsigned int SIZE)
{
	struct ethernetFrame *returnFrame = (struct ethernetFrame *) malloc (sizeof(struct ethernetFrame));
	
	int i;
	for(i=0; i<6; i++) /* Loop through buffer and populate Ethernet struct */
	{
		returnFrame->destAddr[i] = buffer[i];
	}
	
	int spacer = 6;
	for(i=0; i<6; i++)
	{
		returnFrame->sourceAddr[i] = buffer[spacer + i];
	}
	
	spacer = 12;
	for(i=0; i<2; i++)
	{
		returnFrame->etherType[i] = buffer[spacer + i];
	}
	
	spacer = 14;
	for(i=0; i<(SIZE - spacer); i++)
	{
		returnFrame->payload[i] = buffer[spacer + i];
	}
	
	returnFrame->size = SIZE;
	
	for(i=0; i < ETH_FRAME_LEN; i++)
	{
		returnFrame->rawFrame[i] = buffer[i];
	}
	
	startTimer = startTimeBenchmark();
	returnFrame->FCS = crc32(buffer, SIZE);
	returnFrame->fcsBenchmark = getTimeToRun(startTimer);
	
	return returnFrame;
}

void printFrame(struct ethernetFrame *frame)
{
	int i;
	
	printf("Print Destination Address: ");
	for(i=0; i<6; i++)
	{
		printf("%X", (frame->destAddr[i] >> 4));
		unsigned char temp = frame->destAddr[i] << 4;
		temp >>= 4;
		printf("%X", temp);
		if ( i != 5 ) { printf(":"); }
	}
	printf("\nPrint Source Address: ");
	for(i=0; i<6; i++)
	{
		printf("%X", (frame->sourceAddr[i] >> 4));
		unsigned char temp = frame->sourceAddr[i] << 4;
		temp >>= 4;
		printf("%X", temp);
		if ( i != 5 ) { printf(":"); }
	}
	
	printf("\nPrint EtherType: ");
	for(i=0; i<2; i++)
	{
		printf("%X", (frame->etherType[i] >> 4));
		unsigned char temp = frame->etherType[i] << 4;
		temp >>= 4;
		printf("%X", temp);
	}
	descEtherType(frame);
	
	unsigned int crcTemp;
	printf("CRC Value: ");
	// Print 1/8 4 bits
	printf("%X", (frame->FCS >> 28));
	// Print 2/8 4 bits
	crcTemp = frame->FCS << 4; printf("%X", (crcTemp >> 28));
	/* Print 3/8 4 bits */
	crcTemp = frame->FCS << 8; printf("%X", (crcTemp >> 28));
	/* Print 4/8 4 bits */
	crcTemp = frame->FCS << 12; printf("%X", (crcTemp >> 28));
	/* Print 5/8 4 bits */
	crcTemp = frame->FCS << 16; printf("%X", (crcTemp >> 28));
	/* Print 6/8 4 bits */
	crcTemp = frame->FCS << 20; printf("%X", (crcTemp >> 28));
	/* Print 7/8 4 bits */
	crcTemp = frame->FCS << 24; printf("%X", (crcTemp >> 28));
	/* Print 8/8 4 bits */
	crcTemp = frame->FCS << 28; printf("%X", (crcTemp >> 28));
	printf(" (%f seconds)\n", frame->fcsBenchmark);
	
	printf("Payload:\n");
	for(i=0; i<(frame->size - 14); i++)
	{
		printf("%X", (frame->payload[i] >> 4));
		unsigned char temp = frame->payload[i] << 4;
		temp >>= 4;
		printf("%X", temp);
	}
	printf("\n");
	
	printf("Raw Frame:\n");
	for(i=0; i<frame->size; i++)
	{
		printf("%X", (frame->rawFrame[i] >> 4));
		unsigned char temp = frame->rawFrame[i] << 4;
		temp >>= 4;
		printf("%X", temp);
	}
	printf("\n\n");
}

int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray)
{
	if( frame->etherType[0] == etherTypeArray[0]  && frame->etherType[1] == etherTypeArray[1] ) { return 1; }
	else { return 0; }
}

void descEtherType(struct ethernetFrame *frame)
{
	/* IPv4 */
	if( checkEtherType(frame, IP4etherType) ) {printf(" - DOD Internet Protocol (IP)");}
	
	/* IPv6 */
	if( checkEtherType(frame, IP6etherType) ) {printf(" - IP version 6");}
	
	/* ARP Ethertype */
	if( checkEtherType(frame, ARPetherType) ) {printf(" - Address Resolution Protocol (ARP)");}
	
	printf("\n");
}

/*
 *	CRC Calculations
 *	Code taken from http://www.faqs.org/faqs/compression-faq/part1/section-26.html
 */

unsigned int crc32_table[256];
/* Initialized first time "crc32()" is called. If you prefer, you can
 * statically initialize it at compile time. [Another exercise.]
 */

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

/*
 * Build auxiliary table for parallel byte-at-a-time CRC-32.
 */
#define CRC32_POLY 0x04c11db7     /* AUTODIN II, Ethernet, & FDDI */

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
