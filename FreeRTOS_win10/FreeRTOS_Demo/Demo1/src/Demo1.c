#include <include.h>

/*********************************************************************************************************************************
 *                                                     Macro definitions
 *********************************************************************************************************************************

 /* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

 /* The rate at which data is sent to the queue.  The times are converted from
 milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define mainTASK_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 200UL )
#define mainTIMER_SEND_FREQUENCY_MS			pdMS_TO_TICKS( 2000UL )

 /* The number of items the queue can hold at once. */
#define mainQUEUE_LENGTH					( 2 )

/* The values sent to the queue receive task from the queue send task and the
queue send software timer respectively. */
#define mainVALUE_SENT_FROM_TASK			( 100UL )
#define mainVALUE_SENT_FROM_TIMER			( 200UL )

/* This demo uses heap_5.c, and these constants define the sizes of the regions
that make up the total heap.  heap_5 is only used for test and example purposes
as this demo could easily create one large heap region instead of multiple
smaller heap regions - in which case heap_4.c would be the more appropriate
choice.  See http://www.freertos.org/a00111.html for an explanation. */
#define mainREGION_1_SIZE	8201
#define mainREGION_2_SIZE	29905
#define mainREGION_3_SIZE	7607

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask(void* pvParameters);
static void prvQueueSendTask(void* pvParameters);

/*
 * The callback function executed when the software timer expires.
 */
static void prvQueueSendTimerCallback(TimerHandle_t xTimerHandle);

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/* A software timer that is started from the tick hook. */
static TimerHandle_t xTimer = NULL;

static void  prvInitialiseHeap(void)
{
	/* The Windows demo could create one large heap region, in which case it would
	be appropriate to use heap_4.  However, purely for demonstration purposes,
	heap_5 is used instead, so start by defining some heap regions.  No
	initialisation is required when any other heap implementation is used.  See
	http://www.freertos.org/a00111.html for more information.

	The xHeapRegions structure requires the regions to be defined in start address
	order, so this just creates one big array, then populates the structure with
	offsets into the array - with gaps in between and messy alignment just for test
	purposes. */
	static uint8_t ucHeap[configTOTAL_HEAP_SIZE];
	volatile uint32_t ulAdditionalOffset = 19; /* Just to prevent 'condition is always true' warnings in configASSERT(). */
	const HeapRegion_t xHeapRegions[] =
	{
		/* Start address with dummy offsets						Size */
		{ ucHeap + 1,											mainREGION_1_SIZE },
		{ ucHeap + 15 + mainREGION_1_SIZE,						mainREGION_2_SIZE },
		{ ucHeap + 19 + mainREGION_1_SIZE + mainREGION_2_SIZE,	mainREGION_3_SIZE },
		{ NULL, 0 }
	};

	/* Sanity check that the sizes and offsets defined actually fit into the
	array. */
	configASSERT((ulAdditionalOffset + mainREGION_1_SIZE + mainREGION_2_SIZE + mainREGION_3_SIZE) < configTOTAL_HEAP_SIZE);

	/* Prevent compiler warnings when configASSERT() is not defined. */
	(void)ulAdditionalOffset;

	vPortDefineHeapRegions(xHeapRegions);
}

void demo1(void)
{
	const TickType_t xTimerPeriod = mainTIMER_SEND_FREQUENCY_MS;

	/* This demo uses heap_5.c, so start by defining some heap regions.  heap_5 is only used for test and example reasons.
	   Heap_4 is more appropriate.  See http://www.freertos.org/a00111.html for an explanation. */
	prvInitialiseHeap();

	/* Create the queue. */
	xQueue = xQueueCreate(mainQUEUE_LENGTH, sizeof(uint32_t));

	if (xQueue != NULL)
	{
		/* Start the two tasks as described in the comments at the top of this
		file. */
		xTaskCreate(prvQueueReceiveTask,			/* The function that implements the task. */
			"Rx", 							/* The text name assigned to the task - for debug only as it is not used by the kernel. */
			configMINIMAL_STACK_SIZE, 		/* The size of the stack to allocate to the task. */
			NULL, 							/* The parameter passed to the task - not used in this simple case. */
			mainQUEUE_RECEIVE_TASK_PRIORITY,/* The priority assigned to the task. */
			NULL);							/* The task handle is not required, so NULL is passed. */

		xTaskCreate(prvQueueSendTask, "TX", configMINIMAL_STACK_SIZE, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL);

		/* Create the software timer, but don't start it yet. */
		xTimer = xTimerCreate("Timer",				/* The text name assigned to the software timer - for debug only as it is not used by the kernel. */
			xTimerPeriod,		/* The period of the software timer in ticks. */
			pdFALSE,			/* xAutoReload is set to pdFALSE, so this is a one-shot timer. */
			NULL,				/* The timer's ID is not used. */
			prvQueueSendTimerCallback);/* The function executed when the timer expires. */

		xTimerStart(xTimer, 0); /* The scheduler has not started so use a block time of 0. */

		/*< vTaskStartScheduler is called in the RunFreeRTOS_Demo function */
#if 0
		/* Start the tasks and timer running. */
		vTaskStartScheduler();
#endif
	}

	/*< Since the RunFreeRTOS_Demo function is considered as the main for running FreeRTOS application, below loop is commented */
#if 0 
	/* If all is well, the scheduler will now be running, and the following
	line will never be reached.  If the following line does execute, then
	there was insufficient FreeRTOS heap memory available for the idle and/or
	timer tasks	to be created.  See the memory management section on the
	FreeRTOS web site for more details. */
	for (;; );
#endif
}

static void prvQueueSendTask(void* pvParameters)
{
	TickType_t xNextWakeTime;
	const TickType_t xBlockTime = mainTASK_SEND_FREQUENCY_MS;
	const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TASK;

	/* Prevent the compiler warning about the unused parameter. */
	(void)pvParameters;

	/* Initialise xNextWakeTime - this only needs to be done once. */
	xNextWakeTime = xTaskGetTickCount();

	for (;; )
	{
		/* Place this task in the blocked state until it is time to run again.
		The block time is specified in ticks, pdMS_TO_TICKS() was used to
		convert a time specified in milliseconds into a time specified in ticks.
		While in the Blocked state this task will not consume any CPU time. */
		vTaskDelayUntil(&xNextWakeTime, xBlockTime);

		/* Send to the queue - causing the queue receive task to unblock and
		write to the console.  0 is used as the block time so the send operation
		will not block - it shouldn't need to block as the queue should always
		have at least one space at this point in the code. */
		xQueueSend(xQueue, &ulValueToSend, 0U);
	}
}

static void prvQueueSendTimerCallback(TimerHandle_t xTimerHandle)
{
	const uint32_t ulValueToSend = mainVALUE_SENT_FROM_TIMER;

	/* This is the software timer callback function.  The software timer has a
	period of two seconds and is reset each time a key is pressed.  This
	callback function will execute if the timer expires, which will only happen
	if a key is not pressed for two seconds. */

	/* Avoid compiler warnings resulting from the unused parameter. */
	(void)xTimerHandle;

	/* Send to the queue - causing the queue receive task to unblock and
	write out a message.  This function is called from the timer/daemon task, so
	must not block.  Hence the block time is set to 0. */
	xQueueSend(xQueue, &ulValueToSend, 0U);
}

static void prvQueueReceiveTask(void* pvParameters)
{
	uint32_t ulReceivedValue;

	/* Prevent the compiler warning about the unused parameter. */
	(void)pvParameters;

	for (;; )
	{
		/* Wait until something arrives in the queue - this task will block
		indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
		FreeRTOSConfig.h.  It will not use any CPU time while it is in the
		Blocked state. */
		xQueueReceive(xQueue, &ulReceivedValue, portMAX_DELAY);

		/*  To get here something must have been received from the queue, but
		is it an expected value?  Normally calling printf() from a task is not
		a good idea.  Here there is lots of stack space and only one task is
		using console IO so it is ok.  However, note the comments at the top of
		this file about the risks of making Windows system calls (such as
		console output) from a FreeRTOS task. */
		if (ulReceivedValue == mainVALUE_SENT_FROM_TASK)
		{
			printf("Message received from task\r\n");
		}
		else if (ulReceivedValue == mainVALUE_SENT_FROM_TIMER)
		{
			printf("Message received from software timer\r\n");
		}
		else
		{
			printf("Unexpected message\r\n");
		}

		/* Reset the timer if a key has been pressed.  The timer will write
		mainVALUE_SENT_FROM_TIMER to the queue when it expires. */
		if (_kbhit() != 0)
		{
			/* Remove the key from the input buffer. */
			(void)_getch();

			/* Reset the software timer. */
			xTimerReset(xTimer, portMAX_DELAY);
		}
	}
}