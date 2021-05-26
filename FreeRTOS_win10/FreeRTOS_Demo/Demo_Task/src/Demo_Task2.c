#include <include.h>


void Demo_Task2(void)
{

}

/*****************************************************************************************************************************************
 * Changing the task priority:
 * 1. The vTaskPrioritySet() API function can be used to change the priority of any task after the scheduler has been started.
 * 2. vTaskPrioritySet() is only available when INCLUDE_vTaskPrioritySet is set to 1 in FreeRTOSConfig.h
 * 3. uxTaskPriorityGet() API function can be used to query the priority of a task. Note that this function is available only when
 *    INCLUDE_uxTakPriorityGet is set to 1 in FreeRTOSConfig.h
 * 4. A task can use vTaskDelete() API function to delete itself and any other task. This API is available only when INCLUDE_vTaskDelete
 *    is set to 1 in FreeRTOSConfig.h
 * 5. Deleted task do no exist and cannot enter the running state again.
 * 6. It is the responsibility of the idle task to free memory allocated to tasks that have been deleted. Therefor it is important that
 *    applications using vTaskDelete() API function do not completely starve the idle task of all processing time.
 * NOTE:
 * only memory allocated to a task by the kernel itself will be freed automatically when the task is deleted. Any memory or other resources
 * that the implementation of the task allocated must be freed explicitly
 *
 * NOTE:
 * Scheduling algorithm:
 *
 * Recap of task states and events:
 * 1. A task acutally running is in running state. On a single core processor there can only be one task in the running state at any
 *    given point in time.
 * 2. Tasks that are in ready state are available to be selected by the scheduler as the task to enter running state.
 * 3. Task can wait in the blocked state for an event and are automatically moved back to the ready state when the event occurs.
 *    The events are
 *    1. Temporal events - delay (timers)
 *    2. Synchronization events sent from a task or interrupt service routine
 *       a. Task notification
 *       b. Queue
 *       c. Event groups
 *       d. Semaphore
 *       e. Mutex
 *
 * Configuring the scheduling algorithm:
 *
 * 1. The scheduling algorithm is the software routine that decides which ready state task transition into the running state.
 * 2. The scheduling algorithm can changed using configUSE_PREEMPTION and configUSE_TIME_SLICING(not defined in the current version)
 *    configuration constants
 * 3. configUSE_TICKLESS_IDLE is also an important factor in scheduling algorithm but not defined in the current freeRTOS version
 * 4. In all possible configuration the freeRTOS scheduler will ensure tasks that share a priority are selected to enter the running
 *    state in turn. The 'take it in turn' policy is often refered to as 'round robin scheduling' a round robin scheduling does not
 *    guarentee time is shared equally between tasks of equal priority. only that ready state tasks of equal priority will enter
 *    the running state in turn.
 *
 * Prioritized Pre-emptive scheduling with time slicing:
 * 1.  The below configuration sets the FreeRTOS scheduler to use a scheduling algorithm called  "Fixed priority pre-emptive scheduling
 *     with time slicing"
 *     configUSE_PREEMPTION   1
 *     configUSE_TIME_SLICING 1
 * 2. This scheduling algorithm is used by most small RTOS application
 *
 * An explantion of the terms used to describe the scheduling policy:
 *
 * Fixed Priority:
 * 1. Scheduling algorithm described as "Fixed priority" do not change the priority assigned to the tasks being scheduled.
 * 2. But also do not prevent the tasks themselves from changing their own priority or that of other tasks.
 *
 * Pre-emptive:
 * 1. Pre-emptive scheduling algorithm will immediately 'pre-empt' the running task if a task that has a priority higher than the running state
 *    task enters the ready state task.
 * 2. Being pre-empted means being involuntarily (without explicitly yielding or blocking) moved out of running state and into ready state to
 *    allow the different task to enter the running state.
 *
 * Time slicing:
 * 1. Time slicing is used to share processing time between tasks of equal priority. even when the tasks do not excplicitly yield or enter
 *    the blocked state.
 * 2. Scheduling algorithm described as using "Time Slicing" will select the new task to enter the running state at the end of each time slice
 *    if there are other ready state tasks that have the same priority as the running task. A time slice is equal to the time between two RTOS
 *    tick interrupts.
 *
 * IDLE task configuration:
 * 1. if configIDLE_SHOULD_YIELD is set to 0 then the idle task will remain in the running state for the entierity of its time slice
 *    unless it is preempted by a higher priority task
 * 2. if configIDLE_SHOULD_YIELD is set to 1 then the idle task will yield on each iteration of its loop if there are other idle priority
 *    tasks in the ready state.
 *
 * Prioritized Pre-emptive scheduling without time slicing:
 *
 * Prioritized preemptive scheduling without time slicing maintains the same task selection and preemption algorithm as described in the
 * with time slicing algorithm.
 * But does not use time slicing to share processing time between tasks of equal priority.
 *
 * Use below configuration for above algorithm:
 * 1. configUSE_PREEMPTION   1
 * 2. configUSE_TIME_SLICING 0
 *
 * When time slice is not used, then the scheduler will only select a new task to enter the running state on below condition:
 * 1. A higher priority task enters the ready state.
 * 2. The task in running state enters the blocked or suspended state.
 *
 * a. There are fewer task context switches when time slicing not used, therefore turning time slicing off results in a reduction in the
 *    scheduler processing overhead.
 * b. However, turning time slicing off can also result in tasks of equal priority receiving greatly different amounts of processing time
 * c. running the scheduler without time slicing is considered an advanced technique that should only be used by experienced users
 *
 * Co-operative scheduling:
 * FreeRTOS can also support co-operating scheduling, The FreeRTOSConfig.h settings that configure the FreeRTOS scheduler to use
 * co-operative scheduling are
 *
 * configUSE_PREEMPTION   0
 * configUSE_TIME_SLICING any value
 *
 * 1. When co-operative scheduler is used, a context switch will only occur when the running state task enters the blocked state. or the
 *    running state task excplitly yields (manually requests a rechedule) by calling taskYIELD()
 * 2. Tasks are never preempted, so time slicing cannot be used.
 * 3. When the running state task has lower priority than the task in ready state also the preemption will not happen.
 * 4. When the running sate task moved to blocked state, the scheduler will select the task in the ready state based on the priority,
 *    again the selected task will run till it moved to blocked state.
 *
 *****************************************************************************************************************************************/

