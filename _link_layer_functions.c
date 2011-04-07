/*
 *	Functions related to getting link layer data as well as printing link layer raw hexidecimal values
 *
 *	File: _link_layer_functions.c
 */

void getLinkLayerFrame(unsigned char* buffer)
{
	int eth_socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); /* Open a RAW socket with Ethernet Frame included */
	if (eth_socket == -1) { printf("ERROR, Socket did not bind for some reason."); error(1); } /* Error in binding socket */
	
	recv(eth_socket, buffer, ETH_FRAME_LEN, 0); 		/* Read socket into buffer array while recv is not in error */
}
