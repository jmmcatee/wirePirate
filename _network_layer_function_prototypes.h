/* Function Prototyping */
struct ip4Packet *parseIP4Packet(struct ethernetFrame *linkFrame);
void printIP4Packet(struct ip4Packet *packet);
int checkIPPacketType(struct ethernetFrame *linkFrame);
int checkProtocolType(struct ip4Packet *packet, unsigned char protocolType);
