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

/* Custom built header files */
#include "_benchmarks_timers.h"			// Benchmark Header Files
#include "_link_layer_functions.h"		// Ethernet Function Header File
#include "_user_interface.h"			// User Interface Header File

/* Global Definitions */
#define VERSION 0.1
