/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"

/* -p = print the payloads, -r = print the raw frames/packets */
int main(int argc, char *argv[])
{
	int argument;
	for (argument = 0; argument < argc; argument++)
	{
		// printPayload, printRaw
		if ( strcmp(argv[argument], "-p") == 0 ) { printPayload = 1; }
		if ( strcmp(argv[argument], "-r") == 0 ) { printRaw = 1; }
		if ( strcmp(argv[argument], "-pr") == 0 || strcmp(argv[argument], "-rp") == 0 ) { printPayload = 1; printRaw = 1;  }
	}
	
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
