/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"

/* Benchmark Header Files */
#include "_benchmarks_timers.h"

/* Ethernet Function Header File */
#include "_link_layer_functions.h"

/* User Interface Header File */
#include "_user_interface.h"


int main()
{
	printf("--WirePirate-- (Version: %.2f)\n\n", VERSION);
	
	while ( runOptions(printOptions()) )
	{
		// This will run until it is successful or exited
	}
}
