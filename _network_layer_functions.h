 /*
 *	
 *
 *	File: 	_network_layer_functions.h
 *	Author:	Michael McAtee
 */

/* Function Prototyping */
void parseIPPacket(struct ethernetFrame *linkFrame);
struct ip4Packet parseIP4Packet(struct ethernetFrame *linkFrame);
void printIP4Packet(struct ip4Packet packet);
int checkIPPacketType(struct ethernetFrame *linkFrame);
int checkProtocolType(struct ip4Packet, unsigned char *protocolType);

/* C struct to hold IPv4 packet information */
struct ip4Packet
{
	unsigned char ipVersion;
	unsigned char headerLength;
	unsigned char dscpECN;
	unsigned char totalLength[2];
	unsigned char identification[2];
	unsigned char flagsNOffset[2];
	unsigned char TTL;
	unsigned char protocol;
	unsigned char headerChecksum[2];
	unsigned char sourceAddress[4];
	unsigned char destinationAddress[4];
	unsigned char payload[(ETH_FRAME_LEN - 35)];
	unsigned char rawPacket[(ETH_FRAME_LEN - 14)];
};


/* Functions */
void parseIPPacket(struct ethernetFrame *linkFrame)
{

}

struct ip4Packet parseIP4Packet(struct ethernetFrame *linkFrame)
{

}

void printIP4Packet(struct ip4Packet packet)
{

}

int checkIPPacketType(struct ethernetFrame *linkFrame)
{

}

int checkProtocolType(struct ip4Packet, unsigned char *protocolType)
{

}
