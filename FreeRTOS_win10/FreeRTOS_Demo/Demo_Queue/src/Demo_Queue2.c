#include <include.h>


/*****************************************************************************************************************************************
 * Receiving data from multiple sources:
 *
 * 1. It is common in FreeRTOS designs for a task to receive data from more than one source. The receiving task needs to know where the
 *    data came from to determine how the data should be processed.
 * 2. An easy design solution is to use a single queue to transfer structures with both the value of the data and the source of the data
 *    contained in the structure’s fields.
 * 3. Refer image1.JPG
 *
 *****************************************************************************************************************************************/