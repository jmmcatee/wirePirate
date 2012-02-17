/*
 *	WirePirate
 *	
 *	Authors: Chad Dennie, Zach Farr, Michael McAtee, Todd Whetstone
 *	Description: This program will pull a Ethernet frame in binary form from the network card and parse that frame into high layer protocols.
 *		The program will include parsing for Ethernet frames, IPv4 packets, as well as TCP and UDP packets. Ethernet CRCs will be checked as
 *		will checksums for all packets. Timing will be included as well to compare software checks against the built in hardware checks for
 *		Ethernet, IP, and TCP.
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
}
