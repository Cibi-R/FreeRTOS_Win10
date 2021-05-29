#include <include.h>


/**
 * QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
 *
 * @brief  :
 * 1. A queue must be explicitly created before it can be used.
 * 2. Queues are referenced by handles, which are variables of type QueueHandle_t. The xQueueCreate() API function creates a queue and returns
 *    a QueueHandle_t that references the queue it created.
 * 3. FreeRTOS V9.0.0 also includes the xQueueCreateStatic() function, which allocates the memory required to createa queue statically at
 *    compile time
 * 4. The RAM is used to hold both the queue data structures and the items that are contained in the queue. xQueueCreate() will return NULL if
 *    there is insufficient heap RAM available for the queue to be created.
 *
 * @param  :
 * uxQueueLength  :  The maximum number of items that the queue being created can hold at any one time
 * uxItemSize     :  The size in bytes of each data item that can be stored in the queue.
 *
 * @return : If NULL is returned, then the queue cannot be created because there is insufficient heap memory available for FreeRTOS to allocate
 *           the queue data structures and storage area.
 *           A non-NULL value being returned indicates that the queue has been created successfully. The returned value should be stored as the
 *           handle to the created queue.
 *
 * After queue has been created the xQueueReset() API function can be used to return the queue to its original empty state.
 **/

 /**
  * BaseType_t xQueueSendToFront( QueueHandle_t xQueue,const void * pvItemToQueue, TickType_t xTicksToWait );
  *
  * @brief: xQueueSendToFront() is used to send data to the front (head) of a queue.
  *
  * BaseType_t xQueueSendToBack( QueueHandle_t xQueue, const void * pvItemToQueue, TickType_t xTicksToWait );
  *
  * @brief: xQueueSendToBack() is used to send data to the back (tail) of a queue
  *
  * @param:
  * xQueue  : The handle of the queue to which the data is being sent (written). The queue handle will have been returned from the call to
  *           xQueueCreate() used to create the queue.
  * pvItemToQueue : A pointer to the data to be copied into the queue, The size of each item that the queue can hold is set when the queue is
  *                 created, so this many bytes will be copied from pvItemToQueue into the queue storage area.
  * xTicksToWait  : 1. The maximum amount of time the task should remain in the Blocked state to wait for space to become available on the queue,
  *                    should the queue already be full.
  *                 2. Both xQueueSendToFront() and xQueueSendToBack() will return immediately if xTicksToWait is zero and the queue is already full.
  *                    The block time is specified in tick periods, so the absolute time itrepresents is dependent on the tick frequency. The macro
                       pdMS_TO_TICKS() can be used to convert a time specified in milliseconds into a time specified in ticks.
                    3. Setting xTicksToWait to portMAX_DELAY will cause the task to wait indefinitely (without timing out),
                       provided INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h.
  *
  * @return: There are two possible return values:
  *          1. pdPASS
  *             a. pdPASS will be returned only if data was successfully sent to the queue.
  *             b. If a block time was specified (xTicksToWait was not zero), then it is possible the calling task was placed into the
  *                Blocked state, to wait for space to become available in the queue, before the function returned, but data was successfully
  *                written to the queue before the block time expired.
  *          2. errQUEUE_FULL
  *             a. errQUEUE_FULL will be returned if data could not be written to the queue because the queue was already full.
  *             b. If a block time was specified (xTicksToWait was not zero) then the calling task will have been placed into the Blocked
  *                state to wait for another task or interrupt to make space in the queue, but the specified block time expired before that
  *                happened.
  *
  **/

  /**
   * BaseType_t xQueueReceive( QueueHandle_t xQueue,void * const pvBuffer, TickType_t xTicksToWait );
   *
   * @brief: 1. xQueueReceive() is used to receive (read) an item from a queue. The item that is received is removed from the queue.
   *         2. Never call xQueueReceive() from an interrupt service routine. The interrupt-safexQueueReceiveFromISR()
   *
   *
   * @param:
   * QueueHandle_t  :  BaseType_t xQueueReceive( QueueHandle_t xQueue,void * const pvBuffer, TickType_t xTicksToWait );
   * pvBuffer       :  1. A pointer to the memory into which the received data will be copied
   *                   2. The size of each data item that the queue holds is set when the queue is created. The memory pointed to by pvBuffer
   *                      must be at least large enough to hold that many bytes.
   * xTicksToWait   :  1. The maximum amount of time the task should remain in the Blocked state to wait for data to become available on the queue,
   *                      should the queue already be empty
   *                   2. The block time is specified in tick periods, so the absolute time it represents is dependent on the tick frequency.
   *                      The macro pdMS_TO_TICKS() can be used to convert a time specified in milliseconds into a time specified in ticks.
   *                   3. Setting xTicksToWait to portMAX_DELAY will cause the task to wait indefinitely (without timing out) provided
   *                      INCLUDE_vTaskSuspend is set to 1 in FreeRTOSConfig.h.
   *
   * @return : There are two possible return values:
   *           1. pdPASS
   *           2. pdPASS will be returned only if data was successfully read from the queue.
   *           3. If a block time was specified (xTicksToWait was not zero), then it is possible the calling task was placed into the Blocked state,
   *              to wait for data to become available on the queue, but data was successfully read from the queue before the block time expired.
   *
   *           1. errQUEUE_EMPTY
   *           2. errQUEUE_EMPTY will be returned if data cannot be read from the queue because the queue is already empty.
   *           3. If a block time was specified (xTicksToWait was not zero,) then the calling task will have been placed into the Blocked state
   *              to wait for another task or interrupt to send data to the queue, but the block time expired before that happened.
   **/

   /**
    * UBaseType_t uxQueueMessagesWaiting( QueueHandle_t xQueue );
    *
    * @brief: uxQueueMessagesWaiting() is used to query the number of items that are currently in a queue. Never call uxQueueMessagesWaiting()
    *         from an interrupt service routine. The interrupt safe uxQueueMessagesWaitingFromISR() should be used in its place
    *
    * @param:
    * xQueue : The handle of the queue being queried. The queue handle will have been returned from the call to xQueueCreate() used to
    *          create the queue.
    *
    * @return: The number of items that the queue being queried is currently holding. If zero is returned, then the queue is empty.
    *
    **/




/*****************************************************************************************************************************************
 * Queue Management:
 *
 * ‘Queues’ provide a task-to-task, task-to-interrupt, and interrupt-to-task communication mechanism.
 *
 * Characteristics of a queue:
 * 1. A queue can hold a finite number of fixed size data items. The maximum a queue can hold is called its length. Both the length and
 *    size of each data item are set when the queue is created.
 * 2. Queues are normally used as First In First Out (FIFO) buffers, where data is written to the end (tail) of the queue and removed
 *    from the front (head) of the queue.
 * 3. It is also possible to write to the front of a queue, and to overwrite data that is already at the front of a queue
 *
 * There are two ways in which queue behaviour could have been implemented:
 * 1. Queue by copy
 *    Queuing by copy means the data sent to the queue is copied byte for byte into the queue
 *
 * 2. Queue by Reference
 *    Queuing by reference means the queue only holds pointers to the data sent to the queue, not the data itself.
 *
 * FreeRTOS uses the queue by copy method. Queuing by copy is considered to be simultaneously more powerful and simpler to use than
 * queueing by reference because:
 *
 * 1. Stack variable can be sent directly to a queue, even though the variable will not exist after the function in which it is
 *    declared has exited.
 * 2. Data can be sent to a queue without first allocating a buffer to hold the data, and then copying the data into the allocated
 *    buffer
 * 3. The sending task can immediately re-use the variable or buffer that was sent to the queue.
 * 4. The sending task and the receiving task are completely de-coupled the application designer does not need to concern themselves
 *    with which task ‘owns’ the data, or which task is responsible for releasing the data.
 * 5. Queuing by copy does not prevent the queue from also being used to queue by reference. For example, when the size of the data
 *    being queued makes it impractical to copy the data into the queue, then a pointer to the data can be copied into the queue
 *    instead.
 * 6. The RTOS takes complete responsibility for allocating the memory used to store data.
 * 7. In a memory protected system, the RAM that a task can access will be restricted. In that case queueing by reference could only
 *    be used if the sending and receiving task could both access the RAM in which the data was stored. Queuing by copy does not
 *    impose that restriction; the kernel always runs with full privileges, allowing a queue to be used to pass data across memory
 *    protection boundaries.
 *
 * NOTE: Access by multiple task
 * 1. Queues are objects in their own right that can be accessed by any task or ISR that knows of their existence.
 * 2. Any number of tasks can write to the same queue, and any number of tasks can read from the same queue. In practice it is very
 *    common for a queue to have multiple writers, but much less common for a queue to have multiple readers.
 *
 * NOTE: Blocking on Queue Reads
 * 1. When a task attempts to read from a queue, it can optionally specify a ‘block’ time. This is the time the task will be kept in
 *    the Blocked state to wait for data to be available from the queue, should the queue already be empty.
 * 2. A task that is in the Blocked state, waiting for data to become available from a queue, is automatically moved to the Ready
 *    state when another task or interrupt places data into the queue.
 * 3. The task will also be moved automatically from the Blocked state to the Ready state if the specified block time expires before
 *    data becomes available.
 * 4. Queues can have multiple readers, so it is possible for a single queue to have more than one task blocked on it waiting for data.
 *    When this is the case, only one task will be unblocked when data becomes available.
 * 5. The task that is unblocked will always be the highest priority task that is waiting for data. If the blocked tasks have equal
 *    priority, then the task that has been waiting for data the longest will be unblocked.
 *
 * NOTE: Blocking on Queue Writes
 * 1. Just as when reading from a queue, a task can optionally specify a block time when writing to a queue.
 * 2. In this case, the block time is the maximum time the task should be held in the Blocked state to wait for space to become
 *    available on the queue, should the queue already be full.
 * 3. Queues can have multiple writers, so it is possible for a full queue to have more than one task blocked on it waiting to complete
 *    a send operation. When this is the case, only one task will be unblocked when space on the queue becomes available.
 * 4. The task that is unblocked will always be the highest priority task that is waiting for data. If the blocked tasks have equal
 *    priority, then the task that has been waiting for data the longest will be unblocked.
 *
 * NOTE: Blocking on Multiple Queues
 * 1. Queues can be grouped into sets, allowing a task to enter the Blocked state to wait for data to become available on any of the
 *    queues in the set.
 *
 * NOTE: Using a queue
 * NOTE: The xQueueCreate() API Function
 * NOTE: The xQueueSendToBack() and xQueueSendToFront() API Functions
 * NOTE: The xQueueReceive() API Function
 * NOTE: Blocking when receiving from a queue, Refer Example1
 * 1. Example1 demonstrates a queue being created, data being sent to the queue from multiple tasks, and data being received from the queue.
 * 2. The queue is created to hold data items of type int32_t. The tasks that send to the queue do not specify a block time, whereas the task
 *    that receives from the queue does
 * 3. The priority of the tasks that send to the queue are lower than the priority of the task that receives from the queue. This means the queue
 *    should never contain more than one item because, as soon as data is sent to the queue the receiving task will unblock, pre-empt the
 *    sending task, and remove the data—leaving the queue empty once again.
 * 4. Both tasks that send to the queue have an identical priority. This causes the two sending tasks to send data to the queue in turn.
 *
 *****************************************************************************************************************************************/
 