/* Standard included libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Necessary included networking libraries and underlying OS libraries */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <linux/ip.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

/* Global Definitions */
#define VERSION 0.1

/* Global Variables */
unsigned char printPayload 	= 0;
unsigned char printRaw 		= 0;

/* Code header files with prototyping */
#include "_link_layer_function_prototypes.h"
#include "_network_layer_function_prototypes.h"
#include "_user_interface_function_prototypes.h"
#include "_benchmarks_timers_function_prototypes.h"	

/* Custom built code header files */
#include "_benchmarks_timers.h"			// Benchmark Header Files
#include "_link_layer_functions.h"		// Ethernet Function Header File
#include "_network_layer_functions.h"		// Network Function Header File
#include "_user_interface.h"			// User Interface Header File
