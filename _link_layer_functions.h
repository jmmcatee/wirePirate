void getLinkLayerFrame(unsigned char* buffer);
struct ethernetFrame *parseFrame(unsigned char* buffer);
void printFrame(struct ethernetFrame *frame);
int checkEtherType(struct ethernetFrame *frame, unsigned char *etherTypeArray);
void descEtherType(struct ethernetFrame *frame);
