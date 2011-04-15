/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"

/* Benchmark Header Files */
#include "_benchmarks_timers.h"

/* Program Function Header Files */
#include "_link_layer_functions.h"


int main()
{	
	unsigned char buffer[ETH_FRAME_LEN] = "";	/* unsigned (important) byte element size array of ethernet octet length (1514) */
	struct ethernetFrame *ip4Frame;
	struct ethernetFrame *arpFrame;
	
	printf("Capturing Packets...\n\n");

	/* Get ARP Frame */
	unsigned long int frameCount = 1;
	printf("Frame Count:    ");
	do {
		printf("\b\b\b\b%4ld", frameCount); fflush( stdout );
		arpFrame = parseFrame(buffer, getLinkLayerFrame(buffer));
		frameCount++;
	} while( !checkEtherType(arpFrame, ARPetherType) );
	printf("\n");
	printFrame(arpFrame);
	
	/* Get IPv4 Frame */
	frameCount = 1;
	printf("Frame Count:    ");
	do {
		printf("\b\b\b\b%4ld", frameCount); fflush( stdout );
		ip4Frame = parseFrame(buffer, getLinkLayerFrame(buffer));
		frameCount++;
	} while( !checkEtherType(ip4Frame, IP4etherType) );
	printf("\n");
	printFrame(ip4Frame);
}
