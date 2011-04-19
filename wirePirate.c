/*
 *
 *
*/

/* Header File */
#include "wirePirate.h"


int main()
{
	printf("--WirePirate-- (Version: %.2f)\n\n", VERSION);
	
	while ( runOptions(printOptions()) )
	{
		// This will run until it is exited
	}
}
