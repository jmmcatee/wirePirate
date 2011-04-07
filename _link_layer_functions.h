void getLinkLayerFrame(unsigned char* buffer);
struct ethernetFrame *parseFrame(unsigned char* buffer);
void printFrame(struct ethernetFrame *frame);
void descEtherType(struct ethernetFrame *frame);
