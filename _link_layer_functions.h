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
unsigned int crcCalc(unsigned char *buffer, unsigned int SIZE);


unsigned char ARPetherType[2] = {0x08, 0x06}; /* ARP  */
unsigned char IP4etherType[2] = {0x08, 0x00}; /* IPv4  */
unsigned char IP6etherType[2] = {0x86, 0xDD}; /* IPv6  */

struct ethernetFrame
{
	unsigned char destAddr[6];
	unsigned char sourceAddr[6];
	unsigned char etherType[2];
	unsigned int  FCS;
	unsigned int  size;
	
	unsigned char payload[(ETH_FRAME_LEN - 14)];	
	unsigned char rawFrame[ETH_FRAME_LEN];
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
	
	returnFrame->FCS = crcCalc(buffer, SIZE);
	
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
	crcTemp = frame->FCS << 28; printf("%X\n", (crcTemp >> 28));
	
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

unsigned int crcCalc(unsigned char *buffer, unsigned int SIZE)
{
		unsigned int crc  = 0xFFFFFFFF;   // initial contents of LFBSR
        unsigned int poly = 0x04C11DB7;//0xEDB88320;  //0x04C11DB7;   // reverse polynomial

		int b;
        for (b=0; b<SIZE; b++) {
            int temp = (crc ^ buffer[b]) & 0xff;

            // read 8 bits one at a time
            int i;
            for (i = 0; i < 8; i++) {
                if ((temp & 1) == 1) temp = (temp >> 1) ^ poly;
                else                 temp = (temp >> 1);
            }
            crc = (crc >> 8) ^ temp;
        }

        // flip bits
        crc = crc ^ 0xffffffff;
        return crc;
}
