/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"


int main()
{
	printf("--WirePirate-- (Version: %.2f)\n\n", VERSION);
	
	while ( runOptions(printOptions()) )
	{
		// This will run until it is exited
	}
	
	/* Use to test Network Layer Functions
	struct ethernetFrame *frame;
	unsigned char buffer[ETH_FRAME_LEN] = "";
	int s = createSocket();
	do {
		frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
	} while ( !checkEtherType(frame, ip4Type) );
	printFrame(frame);
	*/
}
