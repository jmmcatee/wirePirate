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
		getLinkLayerFrame(buffer);
		
		arpFrame = parseFrame(buffer);	
	} while( arpFrame->etherType[0] != ARPetherType[0]  || arpFrame->etherType[1] != ARPetherType[1] );
	printFrame(arpFrame);
	
	/* Get IPv4 Frame */
	do {
		getLinkLayerFrame(buffer);
		
		ip4Frame = parseFrame(buffer);	
	} while( ip4Frame->etherType[0] != IP4etherType[0]  || ip4Frame->etherType[1] != IP4etherType[1] );
	printFrame(ip4Frame);
}
