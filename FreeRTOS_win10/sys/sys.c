#include <include.h>


int main(void)
{
	/* Initialise the trace recorder.  Use of the trace recorder is optional. See http://www.FreeRTOS.org/trace for more information. */
	vTraceEnable(TRC_START);

	printf("****************** FreeRTOS Applications Windows ******************\n");

	RunFreeRTOS_Demo();

	return 0;
}