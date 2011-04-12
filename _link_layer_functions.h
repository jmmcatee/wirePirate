unsigned int getLinkLayerFrame(unsigned char* buffer);
struct ethernetFrame *parseFrame(unsigned char* buffer, unsigned int SIZE);
void printFrame(struct ethernetFrame *frame);
int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray);
void descEtherType(struct ethernetFrame *frame);
unsigned int crcCalc(unsigned char *buffer, unsigned int SIZE);
