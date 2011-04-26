struct tcpPacket *parseTCPPacket(struct ip4Packet *ipPacket);
void printTCPPacket(struct tcpPacket *tcp);

struct udpPacket *parseUDPPacket(struct ip4Packet *ip4Packet);
void printUDPPacket(struct udpPacket *udpPacket);
