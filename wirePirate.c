/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"

/* Benchmark Header Files */
#include "_benchmarks_timers.h"

/* Ethernet Function Header File */
#include "_link_layer_functions.h"

/* User Interface Header File */
#include "_user_interface.h"


int main()
{	
	unsigned char buffer[ETH_FRAME_LEN] = "";	/* unsigned (important) byte element size array of ethernet octet length (1514) */
	struct ethernetFrame *ip4Frame;
	struct ethernetFrame *arpFrame;
	
	printOptions();
	
	int s = createSocket();
	
	printf("Capturing Packets...\n\n");

	/* Get ARP Frame */
	unsigned long int frameCount = 1;
	do {
		printf("Frame Count: %ld\r", frameCount); fflush( stdout );
		arpFrame = parseFrame(buffer, getLinkLayerFrame(s, buffer));
		frameCount++;
	} while( !checkEtherType(arpFrame, arpType) );
	printf("\n");
	printFrame(arpFrame);
	
	/* Get IPv4 Frame */
	frameCount = 1;
	do {
		printf("Frame Count: %ld\r", frameCount); fflush( stdout );
		ip4Frame = parseFrame(buffer, getLinkLayerFrame(s, buffer));
		frameCount++;
	} while( !checkEtherType(ip4Frame, ip4Type) );
	printf("\n");
	printFrame(ip4Frame);
}
