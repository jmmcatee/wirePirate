/* constants */
struct timeval startTimer;

struct timeval startTimeBenchmark()
{
	struct timeval startTimer;
	gettimeofday(&startTimer, NULL);
	return startTimer;
}

double getTimeToRun(struct timeval startTimer)
{
    /* Define time variables */
    struct timeval endTimer;
    double t1, t2;
    
    /* Start time is passed so now take the ending time */
	gettimeofday(&endTimer, NULL);
    
    /* get the floating point value for how many seconds it took */
    t1=startTimer.tv_sec+(startTimer.tv_usec/1000000.0);
	t2=endTimer.tv_sec+(endTimer.tv_usec/1000000.0);
	
	/* take the floating point value of the end time and substract the start time then print */
    return (t2-t1);
}
