/*
 *
 *
*/

/* Standard included libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Necessary included networking libraries and underlying OS libraries */
#include "_networking_includes.h"

/* Program Function Header Files */
#include "_link_layer_functions.h"

/* Program Function Files */
#include "_link_layer_functions.c"

int main()
{	
	unsigned char buffer[ETH_FRAME_LEN] = "";	/* unsigned (important) byte element size array of ethernet octet length (1514) */
	struct ethernetFrame *ip4Frame;
	struct ethernetFrame *arpFrame;
	
	/* Get ARP Frame */
	do {
		arpFrame = parseFrame(buffer, getLinkLayerFrame(buffer));	
	} while( !checkEtherType(arpFrame, ARPetherType) );
	printFrame(arpFrame);
	
	unsigned int CRCvalue = crcCalc(buffer, arpFrame->size);
	unsigned int temp;
	printf("CRC Value: ");
	/* Print 1/8 4 bits */
	printf("%X", (CRCvalue >> 28));
	/* Print 2/8 4 bits */
	temp = CRCvalue << 4;
	printf("%X", (temp >> 28));
	/* Print 3/8 4 bits */
	temp = CRCvalue << 8;
	printf("%X", (temp >> 28));
	/* Print 4/8 4 bits */
	temp = CRCvalue << 12;
	printf("%X", (temp >> 28));
	/* Print 5/8 4 bits */
	temp = CRCvalue << 16;
	printf("%X", (temp >> 28));
	/* Print 6/8 4 bits */
	temp = CRCvalue << 20;
	printf("%X", (temp >> 28));
	/* Print 7/8 4 bits */
	temp = CRCvalue << 24;
	printf("%X", (temp >> 28));
	/* Print 8/8 4 bits */
	temp = CRCvalue << 28;
	printf("%X", (temp >> 28));
	printf("\n\n");
	
	/* Get IPv4 Frame */
	do {
		ip4Frame = parseFrame(buffer, getLinkLayerFrame(buffer));	
	} while( !checkEtherType(ip4Frame, IP4etherType) );
	printFrame(ip4Frame);
	
	CRCvalue = crcCalc(buffer, ip4Frame->size);
	printf("CRC Value: ");
	/* Print 1/8 4 bits */
	printf("%X", (CRCvalue >> 28));
	/* Print 2/8 4 bits */
	temp = CRCvalue << 4;
	printf("%X", (temp >> 28));
	/* Print 3/8 4 bits */
	temp = CRCvalue << 8;
	printf("%X", (temp >> 28));
	/* Print 4/8 4 bits */
	temp = CRCvalue << 12;
	printf("%X", (temp >> 28));
	/* Print 5/8 4 bits */
	temp = CRCvalue << 16;
	printf("%X", (temp >> 28));
	/* Print 6/8 4 bits */
	temp = CRCvalue << 20;
	printf("%X", (temp >> 28));
	/* Print 7/8 4 bits */
	temp = CRCvalue << 24;
	printf("%X", (temp >> 28));
	/* Print 8/8 4 bits */
	temp = CRCvalue << 28;
	printf("%X", (temp >> 28));
	printf("\n\n");
}
