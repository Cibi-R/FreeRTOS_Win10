#include <include.h>


/*****************************************************************************************************************************************
 * NOTE: Dynamic Memory Allocation (or) Heap
 *
 * Overview:
 * 1. Heap1, we can allocate memory but cannot deallocate
 * 2. Heap2, We can allocate and deallocate, uses best fit algorithm to allocate memory, prone to memory fragmentation
 * 3. Heap3, uses standard library, heap3 makes malloc and free thread safe by temporarily suspending the scheduler
 * 4. Heap4, We can allocate and deallocate, uses first fit algorithm to allocate memory, combine adjacent blocks, less impact of
 *    memory fragmentation
 * 5. Heap5
 *
 * From FreeRTOS kernel V9.0.0 kernel objects can be allocated statically at compile time or dynamically at run time.
 *
 * 1. To make FreeRTOS as easy to use as possible, the kernel object
 *    a. Tasks
 *    b. Queues
 *    c. Semaphores
 *    d. Event groups 
 *    are not statically allocated at compile time, but created dynamically at run time.
 * 2. FreeRTOS allocates RAM each time a kernel object is created, and frees RAM each time a kernel object is deleted This policy
 *    reduces design and planning effort, simplifies the API, and minimizes the RAM footprint.
 *
 * Dynamic memory allocation is a C programming concept, and not a concept that is specific to either FreeRTOS or multitasking.
 * It is relevant to FreeRTOS because kernel objects are allocated dynamically, and the dynamic memory allocation schemes provided by
 * general purpose compilers are not always suitable for real-time applications.
 *
 * Memory can be allocated using the standard C library malloc() and free() functions, but they may not be suitable, or appropriate,
 * for one or more of the following reasons:
 *
 * 1. They are not always available on small embedded systems
 * 2. Their implementation can be relatively large, taking up valuable code space
 * 3. They are rarely thread-safe
 * 4. They are not deterministic; the amount of time taken to execute the functions will differ from call to call
 * 5. They can suffer from fragmentation1
 * 6. They can complicate the linker configuration
 * 7. They can be the source of difficult to debug errors if the heap space is allowed to grow into memory used by other variables
 *
 * NOTE: Options for dynamic memory allocation:
 *
 * NOTE: Example memory allocation:
 * HEAP: Heap1
 * 1. It is common for small dedicated embedded systems to only create tasks and other kernel objects before the scheduler has been
 *    started. When this is the case, memory only gets dynamically allocated by the kernel before the application starts to perform any
 *    real-time functionality, and the memory remains allocated for the lifetime of the application.
 * 2. This means the chosen allocation scheme does not have to consider any of the more complex memory allocation issues, such as
 *    determinism and fragmentation, and can instead just consider attributes such as code size and simplicity
 * 3. Heap_1.c implements a very basic version of pvPortMalloc(), and does not implement vPortFree(). Applications that never delete a
 *    task, or other kernel object, have the potential to use heap_1
 * 4. Some commercially critical and safety critical systems that would otherwise prohibit the use of dynamic memory allocation also
 *    have the potential to use heap_1. Critical systems often prohibit dynamic memory allocation because of the uncertainties
 *    associated with nondeterminism, memory fragmentation, and failed allocations—but Heap_1 is always deterministic, and cannot
 *    fragment memory
 * 5. The heap_1 allocation scheme subdivides a simple array into smaller blocks, as calls to pvPortMalloc() are made. The array is
 *    called the FreeRTOS heap.
 * 6. The total size (in bytes) of the array is set by the definition configTOTAL_HEAP_SIZE within FreeRTOSConfig.h. Defining a
 *    large array in this manner can make the application appear to consume a lot of RAM—even before any memory has been allocated
 *    from the array.
 * Each created task requires a control block (TCB) and a stack to be allocated from the heap
 *
 * From image heap1.JPG - Refer img folder
 * 1. A shows the array before any tasks have been created—the entire array is free
 * 2. B shows the array after one task has been created
 * 3. C shows the array after three tasks have been created.
 *
 *  HEAP: Heap2
 * 1. Heap_2 is retained in the FreeRTOS distribution for backward compatibility, but its use is not recommended for new designs.
 *    Consider using heap_4 instead of heap_2, as heap_4 provides enhanced functionality.
 * 2. Heap_2.c also works by subdividing an array that is dimensioned by configTOTAL_HEAP_SIZE
 * 3. It uses a best fit algorithm to allocate memory and, unlike heap_1, it does allow memory to be freed.
 * 4. Again, the array is statically declared, so will make the application appear to consume a lot of RAM, even before any memory
 *    from the array has been assigned
 *
 * The best fit algorithm ensures that pvPortMalloc() uses the free block of memory that is closest in size to the number of bytes
 * requested. For example,
 * consider the scenario where:
 * 1. The heap contains three blocks of free memory that are 5 bytes, 25 bytes, and 100 bytes, respectively
 * 2. pvPortMalloc() is called to request 20 bytes of RAM.
 *
 * The smallest free block of RAM into which the requested number of bytes will fit is the 25-byte block, so pvPortMalloc() splits the
 * 25-byte block into one block of 20 bytes and one block of 5 bytess, before returning a pointer to the 20-byte block. The new
 * 5-byte block remains available to future calls to pvPortMalloc().
 *
 * 1. Unlike heap_4, Heap_2 does not combine adjacent free blocks into a single larger block, so it is more susceptible to fragmentation
 * 2. However, fragmentation is not an issue if the blocks being allocated and subsequently freed are always the same size.
 *    Heap_2 is suitable for an application that creates and deletes tasks repeatedly, provided the size of the stack allocated
 *    to the created tasks does not change.
 *
 * demonstrates how the best fit algorithm works when a task is created, deleted, and then created again.
 * Referring to heap2.JPG:
 * 1. A shows the array after three tasks have been created. A large free block remains at the top of the array.
 * 2. B shows the array after one of the tasks has been deleted. The large free block at the  top of the array remains.
 *    There are now also two smaller free blocks that were previously allocated to the TCB and stack of the deleted task
 * 3. C shows the situation after another task has been created. Creating the task has resulted in two calls to pvPortMalloc(),
 *    one to allocate a new TCB, and one to allocate the task stack.
 *
 * Heap_2 is not deterministic, but is faster than most standard library implementations of malloc() and free().
 *
 * HEAP: Heap3
 * 1. Heap_3.c uses the standard library malloc() and free() functions, so the size of the heap is defined by the linker configuration,
 *    and the configTOTAL_HEAP_SIZE setting has no affect.
 * 2. Heap_3 makes malloc() and free() thread-safe by temporarily suspending the FreeRTOS scheduler
 *****************************************************************************************************************************************/



