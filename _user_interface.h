/*
 *	This is a library for the user interface portion of wirePirate.
 */

int printOptions();
int runOptions(int option);
void runRandGrabPacket();
int runGrabPacket();


int printOptions()
{	
	int option = 0;
	
	printf("Please choose an option.\n");
	printf(" 1: Grab random packet and frame stack\n");
	printf(" 2: Grab specific frame type stack\n");
	printf(" 3: Grab specific packet type (not implemented)\n");
	printf(" 4: Get a number of packets and frames (not implemented)\n");
	printf(" 0: Quit\n");
	printf(" -> ");
	
	scanf("%d", &option);
	printf("\n");
	
	return option;
}

int runOptions(int option)
{
	switch(option)
	{
		case 1:
			system("clear");
			runRandGrabPacket();
			return 1;
			break;
		case 2:
			system("clear");
			while ( runGrabPacket() ) {/* will run until completed successfully */}
			return 1;
			break;
		case 3:
			printf("Does nothing\n\n");
			return 1;
			break;
		case 4:
			printf("Does nothing\n\n");
			return 1;
			break;
		case 0:
			system("clear");
			printf("Exiting...\n");
			return 0;
			break;
		default:
			printf("Option %d is not a valid choice. Please select again\n", option);
			return 1;
	}
}

void runRandGrabPacket()
{
	int s = createSocket();
	unsigned char buffer[ETH_FRAME_LEN] = "";
	struct ethernetFrame *randomFrame;
	
	printf("Capturing Packets...\n\n");
	randomFrame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
	printFrame(randomFrame);
}

int runGrabPacket()
{
	int frameType = 0;
	unsigned int frameCount = 0;
	
	int s = createSocket();
	unsigned char buffer[ETH_FRAME_LEN] = "";
	struct ethernetFrame *frameWithType;
	
	printf("Please select the EtherType you wish to grab.\n");
	printf(" 1: ARP Frames  (0x0809)\n");
	printf(" 2: IPv4 & IPv6 (0x0800)\n");
	printf(" 0: Quit\n");
	printf(" -> ");
	
	scanf("%d", &frameType);
	printf("\n\n");
	system("clear");
	
	switch(frameType)
	{
		case 1:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( !checkEtherType(frameWithType, arpType) );
			printFrame(frameWithType);
			return 1;
			break;
		case 2:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( !checkEtherType(frameWithType, ip4Type) );
			printFrame(frameWithType);
			return 1;
			break;
		case 0:
			system("clear");  // printf("\n\n");
			return 0;
			break;
		default:
			printf("Error, %d is not a valid option. Please choose again.\n\n", frameType);
			return 1;
			break;
	}
}
