/*
 *	Functions related to getting link layer data as well as printing link layer raw hexidecimal values
 *
 *	File: _link_layer_functions.c
 */

struct ethernetFrame
{
	unsigned char destAddr[6];
	unsigned char sourceAddr[6];
	unsigned char etherType[2];
	unsigned char payload[(ETH_FRAME_LEN - 14)];
};


unsigned char ARPetherType[2] = {0x08, 0x06}; /* ARP  */
unsigned char IP4etherType[2] = {0x08, 0x00}; /* IPv4  */
unsigned char IP6etherType[2] = {0x86, 0xDD}; /* IPv6  */

void getLinkLayerFrame(unsigned char* buffer)
{
	int eth_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); /* Open a RAW socket with Ethernet Frame included */
	if (eth_socket == -1) { printf("ERROR, Socket did not bind for some reason."); error(1); } /* Error in binding socket */
	
	recv(eth_socket, buffer, ETH_FRAME_LEN, 0); 		/* Read socket into buffer array while recv is not in error */
}

struct ethernetFrame *parseFrame(unsigned char* buffer)
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
	for(i=0; i<(ETH_FRAME_LEN - 14); i++)
	{
		returnFrame->payload[i] = buffer[spacer + i];
	}
	
	return returnFrame;
}

void printFrame(struct ethernetFrame *frame)
{
	printf("Print Destination Address: ");
	int i;
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
	
	printf("\nPayload:\n");
	for(i=0; i<(ETH_FRAME_LEN - 14); i++)
	{
		printf("%X", (frame->payload[i] >> 4));
		unsigned char temp = frame->payload[i] << 4;
		temp >>= 4;
		printf("%X", temp);
	}
	printf("\n\n");
}

void descEtherType(struct ethernetFrame *frame)
{
	/* IPv4 */
	if( frame->etherType[0] == IP4etherType[0]  && frame->etherType[1] == IP4etherType[1] ) {printf(" - DOD Internet Protocol (IP)");}
	
	/* IPv6 */
	if( frame->etherType[0] == IP6etherType[0]  && frame->etherType[1] == IP6etherType[1] ) {printf(" - IP version 6");}
	
	/* ARP Ethertype */
	if( frame->etherType[0] == ARPetherType[0]  && frame->etherType[1] == ARPetherType[1] ) {printf(" - Address Resolution Protocol (ARP)");}
}
