/* Function Prototyping */
int createSocket();
unsigned int getLinkLayerFrame(int eth_socket, unsigned char* buffer);
struct ethernetFrame *parseFrame(unsigned char* buffer, unsigned int SIZE);
void printFrame(struct ethernetFrame *frame);
int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray);
void descEtherType(struct ethernetFrame *frame);
unsigned int crc32(unsigned char *buf, unsigned int len);
void init_crc32();
