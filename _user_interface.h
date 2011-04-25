/*
 *	This is a library for the user interface portion of wirePirate.
 */


int printOptions()
{	
	int option = 0;
	
	printf("Please choose an option.\n");
	printf(" 1: Grab random frame stack\n");
	printf(" 2: Grab specific frame or packet type\n");
	printf(" 3: Get a number of packets and frames\n");
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
			while ( runGrabPacket() ) {/* will run until exited */}
			return 1;
			break;
		case 3:
			while ( runGetNumPackets() ) {/* will run until exited */}
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
	printf(" 1: Frame Type: ARP\n");
	printf(" 2: Frame Type: IP\n");
	printf(" 3: Packet Type: IPv4\n");
	printf(" 4: Packet Type: IPv6\n");
	printf(" 5: Packet Type: TCP\n");
	printf(" 6: Packet Type: UDP\n");
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
		case 3:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( checkIPPacketType(frameWithType) != 4 );
			printFrame(frameWithType);
			return 1;
			break;
		case 4:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( checkIPPacketType(frameWithType) != 6 );
			printFrame(frameWithType);
			return 1;
			break;
		case 5:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( !checkProtocolType(frameWithType->ip4payload, tcpType) );
			printFrame(frameWithType);
			return 1;
			break;
		case 6:
			printf("Capturing Packets...\n");
			do {
				frameWithType = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Number of frames parsed: %d\r", frameCount); fflush(stdout);
			} while ( !checkProtocolType(frameWithType->ip4payload, udpType) );
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

int runGetNumPackets()
{
	short int typeToGet = 0;
	int numPackets = 0;
	unsigned int frameCount = 0;
	unsigned int checkedFrame = 0;
	short int frameType = 0;

	int s = createSocket();
	unsigned char buffer[ETH_FRAME_LEN] = "";
	struct ethernetFrame *frame;
	
	printf("Please select the EtherType you wish to grab.\n");
	printf(" 1: Any Frames or Packets\n");
	printf(" 2: Frame Type: ARP\n");
	printf(" 3: Frame Type: IP\n");
	printf(" 4: Packet Type: IPv4\n");
	printf(" 5: Packet Type: IPv6\n");
	printf(" 6: Packet Type: TCP\n");
	printf(" 7: Packet Type: UDP\n");
	printf(" 0: Quit\n");
	printf(" -> ");
	
	scanf("%hd", &typeToGet); printf("\n");
	
	if ( typeToGet != 0 )
	{
		printf("How many frame/packets do you want?\n -> "); scanf("%d", &numPackets); system("clear");
	}
	
	switch(typeToGet)
	{
		case 1:	
			
			printf("Capturing Packets...\n");
			do {
				frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
				frameCount++;
				printf("Frame: %d", frameCount);
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 2:		
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( !checkEtherType(frame, arpType) );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 3:
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( !checkEtherType(frame, ip4Type) );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 4:
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( checkIPPacketType(frame) != 4 );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 5:
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( checkIPPacketType(frame) != 6 );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 6:
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( !checkProtocolType(frame->ip4payload, tcpType) );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 7:
			printf("Capturing Packets...\n\n");
			do {
				do {
					frame = parseFrame( buffer, getLinkLayerFrame(s, buffer) );
					checkedFrame++;
					printf("Checked %d frames...\r", checkedFrame); fflush(stdout);
				} while ( !checkProtocolType(frame->ip4payload, udpType) );
				frameCount++;
				printf("Frame: %d (took %d frames to find)", frameCount, checkedFrame);
				checkedFrame = 0;
				printFrame(frame);
			} while ( frameCount < numPackets );
			return 1;
			break;
		case 0:
			system("clear");
			return 0;
			break;
		default:
			printf("Error, %d is not a valid option. Please choose again.\n\n", typeToGet);
			return 1;
			break;
	}
}
