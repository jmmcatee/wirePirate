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
	unsigned char buffer[ETH_FRAME_LEN] = ""; 						/* unsigned (important) byte element size array of ethernet octet length (1514) */
	
	getLinkLayerFrame(buffer);
	
	int i;
	for(i = 0; i < ETH_FRAME_LEN; i++)
	{
		printf("%X", (buffer[i] >> 4) ); 		/* Print the first four bits as hexidecimal (ex. 10110110 -> 00001011 -> B) */
		unsigned char temp = buffer[i] << 4; 	/* Clear the first four bits of the char (ex. 10110110 -> 01100000) */
		temp >>= 4; 							/* Reset location of four moved bits back to end of char (ex. 01100000 -> 00000110) */
		printf("%X-", temp); 					/* Print cleared last four bits as hexidecimal (ex. 00000110 -> 6) */
	}
	printf("\n\n");
}
