#include <DemoConfig.h>
#include <include.h>

void RunFreeRTOS_Demo(void)
{
#if (ENABLE_DEMO_1 == 1)
	demo1();
#endif
#if (ENABLE_DEMO_TASK == 1)
	Demo_Task1();
#endif

	/*< Start the scheduler */
	vTaskStartScheduler();

	/*< If all is well, the scheduler will now be running, and the following line will never be reached.  If the following line
	   does execute, then there was insufficient FreeRTOS heap memory available for the idle and/or timer tasks	to be created.
	   See the memory management section on the FreeRTOS web site for more details. */
	for (;;);
}