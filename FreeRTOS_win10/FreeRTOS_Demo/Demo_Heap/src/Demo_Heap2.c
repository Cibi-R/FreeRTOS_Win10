#include <include.h>

/**
 * void vPortDefineHeapRegions( const HeapRegion_t * const pxHeapRegions );
 *
 * typedef struct HeapRegion
 * {
 *     The start address of start of memory
 *     uint8_t *pucStartAddress;
 *
 *     The size of the block of memory in bytes
 *     size_t   xSizeInByte;
 * }
 *
 * @brief: Each separate memory areas is described by a structure of type HeapRegion_t. A description of all the available memory
 *         areas is passed into vPortDefineHeapRegions() as an array of HeapRegion_t structures.
 *
 * @param: pxHeapRegions
 *         A pointer to the start of an array of HeapRegion_t structures. Each structure in the array describes the start address
 *         and length of a memory area that will be part of the heap when heap_5 is used.
 *         The HeapRegion_t structures in the array must be ordered by start address; the HeapRegion_t structure that describes the
 *         memory area with the lowest start address must be the first structure in the array, and the HeapRegion_t structure that
 *         describes the memory area with the highest start address must be the last structure in the array.
 *         The end of the array is marked by a HeapRegion_t structure that has its pucStartAddress member set to NULL.
 **/

 /// Below is the example program taken from the reference manual just to understand the concept not to be used in this MCU
#if 0 ///Example1
/* Define the start address and size of the three RAM regions. */
#define RAM1_START_ADDRESS ( ( uint8_t * ) 0x00010000 )
#define RAM1_SIZE ( 65 * 1024 )

#define RAM2_START_ADDRESS ( ( uint8_t * ) 0x00020000 )
#define RAM2_SIZE ( 32 * 1024 )

#define RAM3_START_ADDRESS ( ( uint8_t * ) 0x00030000 )
#define RAM3_SIZE ( 32 * 1024 )

/* Create an array of HeapRegion_t definitions, with an index for each of the three
RAM regions, and terminating the array with a NULL address. The HeapRegion_t
structures must appear in start address order, with the structure that contains the
lowest start address appearing first. */
const HeapRegion_t xHeapRegions[] =
{
 { RAM1_START_ADDRESS, RAM1_SIZE },
 { RAM2_START_ADDRESS, RAM2_SIZE },
 { RAM3_START_ADDRESS, RAM3_SIZE },
 { NULL, 0 } /* Marks the end of the array. */
};


int main(void)
{
    /* Initialize heap_5. */
    vPortDefineHeapRegions(xHeapRegions);
    /* Add application code here. */
}

#endif

#if 0 ///Example2
/**
 *  1. Define the start address and size of the two RAM regions not used by the linker.
 *  2. If needed we can follow the same method used for RAM1
 **/
#define RAM2_START_ADDRESS ( ( uint8_t * ) 0x00020000 )
#define RAM2_SIZE ( 32 * 1024 )

#define RAM3_START_ADDRESS ( ( uint8_t * ) 0x00030000 )
#define RAM3_SIZE ( 32 * 1024 )

 /* Declare an array that will be part of the heap used by heap_5. The array will be
 placed in RAM1 by the linker. */
#define RAM1_HEAP_SIZE ( 30 * 1024 )
static uint8_t ucHeap[RAM1_HEAP_SIZE];

/* Create an array of HeapRegion_t definitions. Whereas in Listing 6 the first entry
described all of RAM1, so heap_5 will have used all of RAM1, this time the first
entry only describes the ucHeap array, so heap_5 will only use the part of RAM1 that
contains the ucHeap array. The HeapRegion_t structures must still appear in start
address order, with the structure that contains the lowest start address appearing
first. */
const HeapRegion_t xHeapRegions[] =
{
 { ucHeap, RAM1_HEAP_SIZE },
 { RAM2_START_ADDRESS, RAM2_SIZE },
 { RAM3_START_ADDRESS, RAM3_SIZE },
 { NULL, 0 } /* Marks the end of the array. */
};

#endif

#if 0
void HeapHelpAPI(void)
{
    /**
     * size_t xPortGetFreeHeapSize( void );
     * @brief: It returns the number of free bytes in the heap at the time the function is called. It can be used to optimize the heap
     *         size. For example, if xPortGetFreeHeapSize() returns 2000 after all the kernel objects have been created, then the
     *         value of configTOTAL_HEAP_SIZE can be reduced by 2000.
     *         xPortGetFreeHeapSize() is not available when heap_3 is used.
     * @return : The number of bytes that remain unallocated in the heap at the time xPortGetFreeHeapSize() is called.
     *
     **/

     /**
      * size_t xPortGetMinimumEverFreeHeapSize( void );
      * @brief  : API function returns the minimum number of unallocated bytes that have ever existed in the heap since the FreeRTOS
      *           application started executing.
      *           The value returned by xPortGetMinimumEverFreeHeapSize() is an indication of how close the application has ever come to
      *           running out of heap space. For example, if xPortGetMinimumEverFreeHeapSize() returns 200, then, at some time since
      *           the application started executing, it came within 200 bytes of running out of heap space.
      *           xPortGetMinimumEverFreeHeapSize() is only available when heap_4 or heap_5 is used.
      * @return : The minimum number of unallocated bytes that have existed in the heap since the FreeRTOS application started
      *           executing
      *
      *
      **/

}
#endif

/*****************************************************************************************************************************************
 * HEAP: Heap4:
 * 1. Like heap_1 and heap_2, heap_4 works by subdividing an array into smaller blocks. As before, the array is statically declared,
 *    and dimensioned by configTOTAL_HEAP_SIZE, so will make the application appear to consume a lot of RAM, even before any memory has
 *    actually been allocated from the array
 * 2. Heap_4 uses a first fit algorithm to allocate memory. Unlike heap_2, heap_4 combines (coalescences) adjacent free blocks of
 *    memory into a single larger block, which minimizes the risk of memory fragmentation
 * 3. The first fit algorithm ensures pvPortMalloc() uses the first free block of memory that is large enough to hold the number of
 *    bytes requested. For example, consider the scenario where:
 *    1.  The heap contains three blocks of free memory that, in the order in which they appear in the array, are 5 bytes, 200 bytes,
 *        and 100 bytes, respectively.
 *    2. pvPortMalloc() is called to request 20 bytes of RAM
 *    3. The first free block of RAM into which the requested number of bytes will fit is the 200-byte block, so pvPortMalloc() splits
 *       the 200-byte block into one block of 20 bytes, and one block of 180 bytes1, before returning a pointer to the 20-byte block.
 *       The new 180-byte block remains available to future calls to pvPortMalloc().
 * 4. Heap_4 combines (coalescences) adjacent free blocks into a single larger block, minimizing the risk of fragmentation, and making
 *    it suitable for applications that repeatedly allocate and free different sized blocks of RAM.
 *
 * Refer Heap4 image for example, Notes is not covered since we understand by the image.
 *
 * Heap_4 is not deterministic, but is faster than most standard library implementations of malloc() and free().
 *
 * Setting a Start Address for the Array Used By Heap_4:
 * 1. Sometimes it is necessary for an application writer to place the array used by heap_4 at a specific memory address. For example, the stack used by a FreeRTOS task is allocated from
 *    the heap, so it might be necessary to ensure the heap is located in fast internal memory, rather than slow external memory.
 * 2. By default, the array used by heap_4 is declared inside the heap_4.c source file, and its startaddress is set automatically
 *    by the linker.
 * 3. However, if the configAPPLICATION_ALLOCATED_HEAP compile time configuration constant is set to 1 in  FreeRTOSConfig.h,
 *    then the array must instead be declared by the application that is using  FreeRTOS. If the array is declared as part of the
 *    application, then the application’s writer can set its start address.
 * 4. If configAPPLICATION_ALLOCATED_HEAP is set to 1 in FreeRTOSConfig.h, then a uint8_t array called ucHeap, and dimensioned by the
 *    configTOTAL_HEAP_SIZE setting, must be declared in one of the application’s source files.
 *
 * The syntax required to place a variable at a specific memory address is dependent on the compiler in use, so refer to your
 * compiler’s documentation.
 *
 * Example:
 * uint8_t ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ( ( section( ".my_heap" ) ) ); for GCC compiler
 *
 * HEAP: Heap5:
 * 1. The algorithm used by heap_5 to allocate and free memory is identical to that used by heap_4. Unlike heap_4, heap_5 is not
 *    limited to allocating memory from a single statically declared  array; heap_5 can allocate memory from multiple and separated
 *    memory spaces.
 * 2. Heap5 is useful when the RAM provided by the system on which FreeRTOS is running does not appear as a single contiguous
 *    (without space) block in the system’s memory map.
 * 3. At the time of writing, heap_5 is the only provided memory allocation scheme that must be explicitly initialized before
 *    pvPortMalloc() can be called. Heap_5 is initialized using the vPortDefineHeapRegions() API function.
 * 4. When heap_5 is used, vPortDefineHeapRegions() must be called before any kernel objects (tasks, queues, semaphores, etc.)
 *    can be created.
 *
 * vPortDefineHeapRegions() API arguments and usage is expalained in the program above.
 *
 * 1. When a project is built, the linking phase of the build process allocates a RAM address to each variable. The RAM available for
 *    use by the linker is normally described by a linker configuration file, such as a linker script
 * 2. In Heap5 image - B side -  it is assumed the linker script included information on RAM1, but did not include information on RAM2
 *    or RAM3. The linker has therefore placed variables in RAM1, leaving only the portion of RAM1
 * 3. The linker has left all of RAM2 and all of RAM3 unused, leaving the whole of RAM2 and the whole of RAM3 available for use by
 *    heap_5
 * 4. The address used by Heap5 and to hold variables may overlap and may face below issue. while selecting start address for Heap5
 *    we may encounter below issue.
 *    1. The start address might not be easy to determine.
 *    2. The amount of RAM used by the linker might change in future builds, necessitating an update to the start address used in the
 *       HeapRegion_t structure.
 *    3. The build tools will not know, and therefore cannot warn the application writer, if the RAM used by the linker and the RAM
 *       used by heap_5 overlap.
 * 5. Example2 demonstrates a more convenient and maintainable example. It declares an array called ucHeap. ucHeap is a normal variable,
 *    so it becomes part of the data allocated to RAM1 by the linker.
 *
 * Advantages of Example2:
 * 1. It is not necessary to use a hard coded start address.
 * 2. The address used in the HeapRegion_t structure will be set automatically, by the linker, so will always be correct, even if the
 *    amount of RAM used by the linker changes in future builds.
 * 3. It is not possible for RAM allocated to heap_5 to overlap data placed into RAM1 by the linker.
 * 4. The application will not link if ucHeap is too big.
 *
 * Heap Related Utility Function:
 * 1. Explained in HeapHelpAPI function above.
 *
 * Malloc Failed Hook Functions:
 * 1. All the example heap allocation schemes can be configured to call a hook (or callback) function if a call to pvPortMalloc()
 *    returns NULL.
 * 2. If configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h, then the application must provide a malloc failed hook function
 *    that has the name and prototype shown below
 *
 * void vApplicationMallocFailedHook( void );
 *
 *****************************************************************************************************************************************/
