#include <include.h>

/**
* TimerHandle_t xTimerCreate( const char * const pcTimerName,
*                             TickType_t xTimerPeriodInTicks,
*							  UBaseType_t uxAutoReload,
*                             void * pvTimerID,
*                             TimerCallbackFunction_t pxCallbackFunction );
* 
* pcTimerName      :  A descriptive name for the timer. This is not used by FreeRTOS in any way. It is included purely as a debugging aid.
*					  Identifying a timer by a human readable name is much simpler than attempting to identify it by its handle.
* xTimerPeriodInTicks : The timer’s period specified in ticks. The pdMS_TO_TICKS() macro can be used to convert a time specified in
*                       milliseconds into a time specified in ticks.
* uxAutoReload     :  Set uxAutoReload to pdTRUE to create an auto-reload timer. Set uxAutoReload to pdFALSE to create a one-shot timer.
* pvTimerID        :  Each software timer has an ID value. The ID is a void pointer, and can be used by the application writer for any
*                     purpose. The ID particularly useful when the same callback function is used by more than one software timer, as it
*                     can be used to provide timer specific storage. Use of a timer’s ID is demonstrated in an example within this 
*					  chapter.
* pxCallbackFunction : Software timer callback functions are simply C functions that conform to the prototype shown in Listing 72.
*                      The pxCallbackFunction parameter is a pointer to the function (in effect, just the function name) to use as the
*                      callback function for the software timer being created.
* 
* return :            1. If NULL is returned, then the software timer cannot be created because there is insufficient heap memory available
*                        for FreeRTOS to allocate the necessary data structure.
*                     2. A non-NULL value being returned indicates that the software timer has been created successfully. The returned value
*                        is the handle of the created timer.
***/

/**
* BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait );
* 
* xTimer     : The handle of the software timer being started or reset. The handle will have been returned from the call to xTimerCreate()
*              used to create the software timer.
* 
* xTicksToWait : 1. xTimerStart() uses the timer command queue to send the ‘start a timer’ command to the daemon task. xTicksToWait
*                   specifies the maximum amount of time the calling task should remain in the Blocked state to wait for space to become
*				    available on the timer command queue, should the queue already be full.
*                2. xTimerStart() will return immediately if xTicksToWait is zero and the timer command queue is already full
*                3. The block time is specified in tick periods, so the absolute time it represents is dependent on the tick frequency.
*                   The macro pdMS_TO_TICKS() can be used to convert a time specified in milliseconds into a time specified in ticks.
*                4. If INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h then setting xTicksToWait to portMAX_DELAY will result in the
*                   calling task remaining in the Blocked state indefinitely (without a timeout) to wait for space to become available in
*                   the timer command queue
* 
* Returned value  :
*
* 1. pdPASS will be returned only if the ‘start a timer’ command was successfully sent to the timer command queue.
* 2. If the priority of the daemon task is above the priority of the task that called xTimerStart(), then the scheduler will ensure the
*    start command is processed before xTimerStart() returns. This is because the daemon task will pre-empt the task that called
*    xTimerStart() as soon as there is data in the timer command queue.
* 3. If a block time was specified (xTicksToWait was not zero), then it is possible the calling task was placed into the Blocked state
*    to wait for space to become available in the timer command queue before the function returned, but data was successfully written to
*    the timer command queue before the block time expired.
* 
* pdFALSE
* 1. pdFALSE will be returned if the ‘start a timer’ command could not be written to the timer command queue because the queue was 
*    already full.
* 2. If a block time was specified (xTicksToWait was not zero) then the calling task will have been placed into the Blocked state to wait
*    for the daemon task to make room in the timer command queue, but the specified block time expired before that happened
* 
*/


/* The periods assigned to the one - shot and auto - reload timers are 3.333 second and half a
second respectively. */

#define mainONE_SHOT_TIMER_PERIOD    pdMS_TO_TICKS(3333)
#define mainAUTO_RELOAD_TIMER_PERIOD pdMS_TO_TICKS(500)

static void prvOneShotTimerCallback(TimerHandle_t xTimer);
static void prvAutoReloadTimerCallback(TimerHandle_t xTimer);

/*< Just to count the no of times the timer callback functions are getting called. */
TickType_t ulCallCount = 0;

void Demo_Timer1(void)
{
	TimerHandle_t xAutoReloadTimer, xOneShotTimer;
	BaseType_t xTimer1Started, xTimer2Started;
	/* Create the one shot timer, storing the handle to the created timer in xOneShotTimer. */
	xOneShotTimer = xTimerCreate(
		/* Text name for the software timer - not used by FreeRTOS. */
		"OneShot",
		/* The software timer's period in ticks. */
		mainONE_SHOT_TIMER_PERIOD,
		/* Setting uxAutoRealod to pdFALSE creates a one-shot software timer. */
		pdFALSE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvOneShotTimerCallback);


	/* Create the auto-reload timer, storing the handle to the created timer in xAutoReloadTimer. */
	xAutoReloadTimer = xTimerCreate(
		/* Text name for the software timer - not used by FreeRTOS. */
		"AutoReload",
		/* The software timer's period in ticks. */
		mainAUTO_RELOAD_TIMER_PERIOD,
		/* Setting uxAutoRealod to pdTRUE creates an auto-reload timer. */
		pdTRUE,
		/* This example does not use the timer id. */
		0,
		/* The callback function to be used by the software timer being created. */
		prvAutoReloadTimerCallback);

	/* Check the software timers were created. */
	if ((xOneShotTimer != NULL) && (xAutoReloadTimer != NULL))
	{
		/* Start the software timers, using a block time of 0 (no block time). The scheduler has
		not been started yet so any block time specified here would be ignored anyway. */
		xTimer1Started = xTimerStart(xOneShotTimer, 0);
		xTimer2Started = xTimerStart(xAutoReloadTimer, 0);
		/* The implementation of xTimerStart() uses the timer command queue, and xTimerStart()
		will fail if the timer command queue gets full. The timer service task does not get
		created until the scheduler is started, so all commands sent to the command queue will
		stay in the queue until after the scheduler has been started. Check both calls to
		xTimerStart() passed. */
		if ((xTimer1Started == pdPASS) && (xTimer2Started == pdPASS))
		{
#if 0 //Scheduler will be started in the Demo_Main function
			/* Start the scheduler. */
			vTaskStartScheduler();
#endif
		}
	}
#if 0
	/* As always, this line should not be reached. */
	for (;; );
#endif 
}

static void prvOneShotTimerCallback(TimerHandle_t xTimer)
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf("One-shot timer callback executing %d\n", xTimeNow);
	/* File scope variable. */
	ulCallCount++;
}

static void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
	TickType_t xTimeNow;

	/* Obtain the current tick count. */
	xTimeNow = xTaskGetTickCount();

	/* Output a string to show the time at which the callback was executed. */
	printf("Auto-reload timer callback executing : %d\n", xTimeNow);

	ulCallCount++;
}


/******************************************************************************************************************************************
* Software Timer Management
* 
* 1. Software timers are used to schedule the execution of a function at a set time in the future, or periodically with a fixed frequency.
*    The function executed by the software timer is called the software timer’s callback function.
* 2. Software timers are implemented by, and are under the control of, the FreeRTOS kernel. They do not require hardware support, 
*    and are not related to hardware timers or hardware counters.
* 3. Note that, in line with the FreeRTOS philosophy of using innovative design to ensure maximum efficiency, software timers do 
*    not use any processing time unless a software timer callback function is actually executing.
* 
* Software timer functionality is optional. To include software timer functionality:
* 
* 1. Build the FreeRTOS source file FreeRTOS/Source/timers.c as part of your project.
* 2. Set configUSE_TIMERS to 1 in FreeRTOSConfig.h.
* 
* Software Timer Callback Functions
* 
* 1. Software timer callback functions are implemented as C functions. The only thing special about them is their prototype, 
*    which must return void, and take a handle to a software timer as its only parameter
* 
* void ATimerCallback( TimerHandle_t xTimer );
* 
* 2. Software timer callback functions execute from start to finish, and exit in the normal way. They should be kept short, and must not 
*    enter the Blocked state
* 3. As will be seen, software timer callback functions execute in the context of a task that is created automatically when the FreeRTOS 
*    scheduler is started. Therefore, it is essential that software timer callback functions never call FreeRTOS API functions that will
*    result in the calling task entering the Blocked state. It is ok to call functions such as xQueueReceive(), but only if the function’s
*    xTicksToWait parameter (which specifies the function’s block time) is set to 0. It is not ok to call functions such as vTaskDelay(),
*    as calling vTaskDelay() will always place the calling task into the Blocked state.
* 
* Attributes and States of a Software Timer, Refer Img1
* 
* 1. Period of a Software Timer NOTE: 
*    a. A software timer’s ‘period’ is the time between the software timer being started, and the software timer’s callback function 
*       executing.
* 2. One-shot and Auto-reload Timers
*    a. One-shot timers
*    b. Auto-reload timers
* a. One-shot timers
*    i. Once started, a one-shot timer will execute its callback function once only. A one-shot timer can be restarted manually,
*       but will not restart itself. 
* b. Auto-reload timers
*    i. Once started, an auto-reload timer will re-start itself each time it expires, resulting in periodic execution of its callback
*       function.
* 
* Software Timer States
* 
* 1. Dormant
* 2. Running
* 
* 1. Dormant
*    a. A Dormant software timer exists, and can be referenced by its handle, but is not running, so its callback functions will not execute
* 2. Running
*    a. A Running software timer will execute its callback function after a time equal to its period has elapsed since the software timer
*       entered the Running state, or since the software timer was last reset.
* 3. Refer img2
* 4. The xTimerDelete() API function deletes a timer. A timer can be deleted at any time.
* 
* The context of a software timer
* 
* The RTOS Daemon (Timer Service) Task
* 1. All software timer callback functions execute in the context of the same RTOS daemon (or ‘timer service’) task1.
* 2. The daemon task is a standard FreeRTOS task that is created automatically when the scheduler is started. Its priority and stack
*    size are set by the configTIMER_TASK_PRIORITY and configTIMER_TASK_STACK_DEPTH compile time configuration constants respectively. 
*    Both constants are defined within FreeRTOSConfig.h
* 3. Software timer callback functions must not call FreeRTOS API functions that will result in the calling task entering the Blocked state,
*    as to do so will result in the daemon task entering the Blocked state.
* 
* The Timer Command Queue
* 1. Software timer API functions send commands from the calling task to the daemon task on a queue called the ‘timer command queue’. 
*    This is shown in Figure 41. Examples of commands include ‘start a timer’, ‘stop a timer’ and ‘reset a timer’. 
* 2. The timer command queue is a standard FreeRTOS queue that is created automatically when the scheduler is started. The length of the 
*    timer command queue is set by the configTIMER_QUEUE_LENGTH compile time configuration constant in FreeRTOSConfig.h.
* 3. The task used to be called the ‘timer service task’, because originally it was only used to execute software timer callback functions.
*    Now the same task is used for other purposes too, so it is known by the more generic name of the ‘RTOS daemon task’
* 4. Refer img3
* 
* Daemon Task Scheduling:
* Refer Manual - 1 Page, Easy :)
* Page No. 155
* 
* Creating and Starting a Software Timer
* 
* 1. FreeRTOS V9.0.0 also includes the xTimerCreateStatic() function, which allocates the memory required to create a timer statically
*    at compile time
* 2. A software timer must be explicitly created before it can be used.
* 3. Software timers are referenced by variables of type TimerHandle_t. xTimerCreate() is used to create a software timer and returns
*    a TimerHandle_t to reference the software timer it creates.
* 4. Software timers are created in the Dormant state.
* 5. Software timers can be created before the scheduler is running, or from a task after the scheduler has been started.
* 
* The xTimerStart() API Function
* 1. xTimerStart() is used to start a software timer that is in the Dormant state, or reset (re-start) a software timer that is in the
*    Running state. xTimerStop() is used to stop a software timer that is in the Running state. Stopping a software timer is the same as
*    transitioning the timer into the Dormant state.
* 2. xTimerStart() can be called before the scheduler is started, but when this is done, the software timer will not actually start 
*    until the time at which the scheduler starts.
* 3. Never call xTimerStart() from an interrupt service routine. The interrupt-safe version xTimerStartFromISR() should be used in its place.
* 
* 4. If xTimerStart() is called before the scheduler has been started then the value of xTicksToWait is ignored, and xTimerStart() behaves
*    as if xTicksToWait had been set to zero.
* 
* Returned value  :
* 
* 1. pdPASS will be returned only if the ‘start a timer’ command was successfully sent to the timer command queue.
* 2. If the priority of the daemon task is above the priority of the task that called xTimerStart(), then the scheduler will ensure the
*    start command is processed before xTimerStart() returns. This is because the daemon task will pre-empt the task that called 
*    xTimerStart() as soon as there is data in the timer command queue.
* 3. 
******************************************************************************************************************************************/