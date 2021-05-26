#include <include.h>

TaskHandle_t Task1_Handle = NULL;

static void FreeRTOS_Task_1_Create(void);
static void FreeRTOS_Task_2_Create(void);
static void FreeRTOS_Task_1(void* pvParameters);
static void FreeRTOS_Task_2(void* pvParameters);

void Demo_Task1(void)
{
	FreeRTOS_Task_1_Create();

	FreeRTOS_Task_2_Create();
}

static void FreeRTOS_Task_1_Create(void)
{
	/**
	 * BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
	 * 						   const char * const pcName,
	 *                         uint16_t usStackDepth,
	 *                         void *pvParameters,
	 *                         UBaseType_t uxPriority,
	 *                         TaskHandle_t *pxCreatedTask);
	 * pvTaskCode    : Tasks are C functions that never exit, as such, are normally implemened as infinite loop
	 *                  In essence TaskFunction_t is a pointer to a task function
	 * pcName        : A descriptive name for task. This is not used by FreeRTOS in any way. It is included purely as
	 *                 debugging aid.
	 * usStackDepth  : Each task has its own stack space that is allocated by kernel to the task. The value specified the
	 *                 number words the stack can hold, not the number of bytes.
	 *                 The stack depth multiplied by the stack width(4 - word size) mush not exceed the maximum value
	 *                 that can be contained in a variable of type uint16_t
	 *                 The size of the stack used by the idle task is defined by the application-defined constant
	 *                 configMINIMAL_STACK_SIZE
	 *                 There is not easy way to determin the stack space required by a task. It is possible to calculate
	 *                 but most users will simply assign what think is a reasonable value.
	 * pvParameters  : Task functions accept a parameter of type pointer to void*. The value assigned to pvParameters is
	 *                 the value passed into the task.
	 * uxPriority    : Defines the priority at which the task will execute. Priorities can be assigned from zero which is
	 *                 the lowest priority, to configMAX_PRIORITIES-1. passing a uxPriority value above configMAX_PRIORITIES
	 *                 will result in maximum legitimate value.
	 * pxCreatedTask : It can be used to pass out a handle to the task being created. This handle then be used to reference
	 *                 the task in API calls that, for example, change the task priority or delete the task.
	 *                 We can also pass NULL if we do not require the handle of the task.
	 *
	 * return value  : There are two possible return values
	 *                 1. pdPASS
	 *                 Indicates that the task has been created successfully
	 *                 2. pdFAIL
	 *                 Indicates that the task has not been created because there is insufficient heap memory available for
	 *                 FreeRTOS to allocate enough RAM to hold the task data structure and stack.
	 *
	 * NOTE:
	 * Tasks should be created before starting the scheduler, vTaskStartScheduler(); API is used to start the scheduler.
	 **/

	 /*< Creating the task. */
	if (xTaskCreate(FreeRTOS_Task_1, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2U, &Task1_Handle) != pdPASS)
	{
		/*< Task Creation failed. */
		while (1);
	}
}

void FreeRTOS_Task_2_Create(void)
{
	if (xTaskCreate(FreeRTOS_Task_2, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2U, NULL))
	{
		while (1);
	}
}

void FreeRTOS_Task_1(void* pvParameters)
{
	volatile uint8_t taskRunning = pdPASS;

	while (taskRunning)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
		printf("%s : Task1 running\n", __LINE__);
	}

	/*< A task should be deleted before the exit, the NULL parameter indicates that the calling function should be deleted. */
	vTaskDelete(NULL);
}


void FreeRTOS_Task_2(void* pvParameters)
{
	uint8_t xThreadStatus = 1u;

	TickType_t xLastWakeupTime;

	xLastWakeupTime = xTaskGetTickCount();

	while (xThreadStatus)
	{
		/**
		 * void vTaskDelayUntil( TickType_t *pxPreviousWakeTime,
		 *                       TickType_t xTimeIncrement
		 * );
		 * pxPreviousWakeTime :  This parameter is named on the assumption that vTaskDelayUntil() is being used to implement a
		 *                       a task that executes periodically and with a fixed frequency. In this case pxPrevioudWakeTime
		 *                       holds the time at which the task has left the blocked state, This time is used as a reference point
		 *                       to calculate the time at which the task should next leave the blocked state.
		 *                       Since we passing reference, it is updated automatically inside vTaskDelayUntil, but must be
		 *                       initialized to the
		 *                       current tick count before it is used for the first time.
		 * xTimeIncrement    :   This parameter is also named on the assumption that vTaskDelayUntil() is being used to implement a
		 *                       task that executes periodically and with a fixed frequency.
		 *                       The value is specified in ticks. The macro pdMS_TO_TICKS() can be used to convert a time specified in
		 *                       milleseconds into a time specified in ticks
		 **/

		/**
		 * This task should execute every 10 millisecond exactly
		 **/
		vTaskDelayUntil(&xLastWakeupTime, pdMS_TO_TICKS(500));

		printf("%s : Task2 running\n", __LINE__);
	}

	vTaskDelete(NULL);
}

/*********************************************************************************************************************************
 * NOTE: Top level task states
 * 1. An application can consist of many tasks. If the processor running the application contains a single core, then only one task
 *    can be executing at any given time. This implies that a task can exist in one of two states. Running and not running. Not running
 *    actually contains a number of sub states.
 * 2. When a task is in the running state the processor is executing the task's code. when a task is not in running state, the task is
 *    dormant, its status having been stored ready for it to resume execution the next time the scheduler decides it should enter
 *    running state.
 *
 * NOTE: Task Priority
 * 1. The uxPriority parameter of the xTaskCreate() (explained above) API function assigns an initial priority of the task being created.
 * 2. The priority can be changed after the scheduler has been started by using the vTaskPrioritySet() API function
 * 3. Maximum number of available priority is set by the application-defined configMAX_PRIORITIES constant in FreeRTOSConfig.h
 *    with 0 being the lowest priority possible, therefore the range of available priorities is 0 to configMAX_PRIORITIES-1
 * 4. Any number of task can share the same priority. The freeRTOS scheduler can use two one of two methods to decide which task
 *    will be in the running state.
 *
 * the maximum value to which configMAX_PRIORITIES can be set depends on the methods used.
 * 1. Generic Method
 * 2. Architecture optimized method
 *
 * Generic Method
 * The generic method is implemented in C, and can be used with all the FreeRTOS architure ports. when the generic method is used
 * FreeRTOS does not limit the maximum value to which configMAX_PRIORITIES can be set. it is always advisable to keep it minimum
 * necessary because the highest its value, the more RAM memory will be consumed and the longer the worst case execution time.
 *
 * The generic method will be used if configUSE_PORT_OPTIMISED_TASK_SELECTION is set to 0 in FreeRTOSConfig.h or if it is left
 * undefined or if the generic method is the only method provided for the FreeRTOS port in use.
 *
 * Architecture Optimized method
 * The architecture optimized method uses a small amount of assembler code. and is faster than the generic code. The
 * configMAX_PRIORITIES setting does not affect the worst case execution time. and configMAX_PRIORITIES cannot be greater than 32
 * This method will be used if configUSE_PORT_OPTIMISED_TASK_SELECTION is set to 1 in FreeRTOSConfig.h
 *
 * Not all FreeRTOS ports provide an architure optimized method.
 *
 * NOTE:
 * The FreeRTOS scheduler will always ensure the highest priority task that is able to run is the task selected to enter the
 * running state. Where more than one task of the same priority is able to run. the scheduler will transition each task into and out
 * of the running state (for every time slice), in turn.
 *
 * NOTE: Time measurement and Tick interrupt
 * 1. Tasks will be executed for a time slice, entering the running start at the start of the time slice and exiting the time slice at
 *    the end of the time slice. To be able to select the next task to run, the scheduler must run at the end of each time slice
 * 2. A periodic interrupt called tick interrupt is used for this purpose. the length of the time slice is effectively set by the
 *    tick interrupt frequency. which is configured by application defined constant configTICK_RATE_HZ in FreeRTOSConfig.h
 * 3. if configTICK_RATE_HZ is 100, then the time slice is 10ms
 * 4. FreeRTOS APIs always specifiy time in multiples of tick periods, which are often refered to as ticks. the pdMS_TO_TICKS() macro
 *    converts a time specified in milliseconds into a time specified in ticks. The resolution available depends on the defined tick
 *    frequency, and pdMS_TO_TICKS() cannot be used if tick frequency is above 1KHz
 * ex. TickType_t xTimeInTicks = pdMS_TO_TICKS(200); xTimeInTicks being set to the number of tick periods that are equivalent to
 *                                                   200 milleseconds.
 * 5. It is not recommended to specify times in ticks directly, but instead to use the pdMS_TO_TICKS(), and in doing so, ensuring
 *    time specified within the application do not change if the tick frequency is changed.
 * 6. The ‘tick count’ value is the total number of tick interrupts that have occurred since the scheduler was started by User application
 *    do not have to consider overflow when specifying delay periods, as time consistency is managed internally by FreeRTOS
 *
 * IMPORTANT:
 * The end of time slice is not the only place that the scheduler can select a new task to run. The scheduler will select a new task
 * to run immediately after the currently executing task enters the blocked state, or when the interrupt moves higher priority task
 * into the ready state.
 *
 * The scheduler will always be executing at the end of every time slice, and select the task which has high priority that is able to
 * run
 *
 * NOTE: Expanding the 'Not Running' State
 * 1. Task designed in such a way that it is always in a running state(higher priority and not blocking API calls), this type of tasks
 *    are called continuous running task.
 * 2. This type of tasks has limited usefulness, because they can only be created at the very lowest priority. If they run at any
 *    other priority they will prevent tasks of lower priority ever running at all.
 * 3. To make the tasks useful they must be re-written to be event driven. therefore event driven means that tasks can be created at
 *    different priorities without the highest priority tasks starving all the lower priority tasks of processing time.
 *
 * Blocked State:
 * A task that is waiting for a event is said to be in the blocked state. which is sub state of not running state.
 *
 * Tasks can enter the blocked state to wait for two different types of events
 * 1. Temporal events - The event being either a delay period expiring, or an absolute time being reached.
 * 2. Synchronization events - where the events originate from another tasks or interrupt
 *
 * FreeRTOS covers broad range of synchronizaiton events:
 * 1. Queues
 * 2. Binary semaphores
 * 3. Counting Semahores
 * 4. Mutexes
 * 5. Recursive mutex
 * 6. Event groups
 * 7. Direct to task notifications
 *
 * Suspended State:
 * 1. Suspended is also a sub-state of not running state. Tasks in the suspended state are not available to the scheduler
 * 2. The only way into the suspended state is through a call to the vTaskSuspend() API function. the only way out being through a call
 *    to the vTaskResume() or xTaskResumeFromISR() API functions. Most application do not use suspended state.
 *
 * Ready State:
 * 1. Tasks that are not in the "not running" state but are not blocked or suspended are said to be in the ready state. They are able
 *    run and therefore "ready" to run but not currently in the running state.
 *
 * Delay Function:
 * NOTE: Delay API functions are available only when INCLUDE_vTaskDelay is set to 1 in FreeRTOSConfig.h
 * 1. vTaskDelay API puts the task into blocked state.
 * 2. vTaskDelayUntil() this API is used where you want your task to execute periodically with a fixed frequency.
 *
 *
 * IDLE Task:
 * 1. The IDLE task is created automatically when the scheduler is started (vStartTaskScheduler()), To ensure there is always at least one
 *    task that is able to run (at least one task in the ready state)
 * 2. No application task that can be selected to enter the running state is the case when the idle task will run. The amount of
 *    processing time allocated to the idle task is a measure of the spare processing capacity in the system.
 * 3. Using an RTOS can significantly increase the spare processing capacity simply by allowing an application to be completely event
 *    driven.
 * 4. The idle task has the lowest possible priority that is 0.
 *
 * NOTE:
 * If an application uses the vTaskDelete() API function then it is essential that the idle task is not starved of  processing time
 * This is because the idle task is responsible for cleaning up kernel resources after a task has been deleted.
 *
 * IDLE hook function:
 * It is possible to add application specific functionality directly into the idle task through the use of an idle hook (or callback)
 * function. a function that is called automatically by the idle task once per iteration of the idle task loop.
 *
 * IMPORTANT:
 * This is the case the MCU on which FreeRTOS is executing will be placed into a low power mode if none of the tasks created by the
 * application are able to run.
 *
 * Command uses of idle hook function:
 * 1. Executing low priority, background or contrinuous processing functionality
 * 2. Measuring the amount of spare processing time
 * 3. Placing the processor into low power state.
 *
 * IMPORTANT:
 * Limitations of idle task hook function:
 * 1. An idle task hook function must never attempt to block or suspend. this will cause scenario where no tasks are available to enter
 *    the running state.
 * 2. If the application makes use of vTaskDelete() API, the idle task hook must always return to its caller within a resonable time
 *    period. This is becuase the idle task is responsible for cleaning up kernel resources after a task has been deleted.
 *
 * IMPORTANT: configUSE_IDLE_HOOK must be set to 1 in FreeRTOSConfig.h for the hook function to get called.
 *
 * Idle task hook functions must have the name and prototype:
 * void vApplicationIdleHook( void );
 *
 * Reference:
 * http://dev.ti.com/tirex/explore/node?node=ADznPZ0t1iqMbLyS3k3ZFA__fc2e6sr__LATEST#:~:text=The%20TI%2DRTOS%20kernel%20supports,scheduling%20with%20Tasks%20if%20desired.
 *
 *
 ***********************************************************************************************************************************/