#ifndef __INCLUDE_H__
#define __INCLUDE_H__

/*< Standard libraries */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdint.h>

#include <intrin.h>

/*< FreeRTOS header files */
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <queue.h>

/*< Demo application header files */
#include "FreeRTOS_Demo/Demo_Main/api/Demo_main.h"
#include "FreeRTOS_Demo/Demo_Task/api/Demo_Task1.h"
#include "FreeRTOS_Demo/Demo_Task/api/Demo_Task2.h"
#include "FreeRTOS_Demo/Demo_Heap/api/Demo_Heap1.h"
#include "FreeRTOS_Demo/Demo_Heap/api/Demo_Heap2.h"
#include "FreeRTOS_Demo/Demo_Queue/api/Demo_Queue1.h"
#include "FreeRTOS_Demo/Demo_Queue/api/Demo_Queue2.h"
#include "FreeRTOS_Demo/Demo_Timer/api/Demo_Timer1.h"
#include "FreeRTOS_Demo/Demo1/api/Demo1.h"

#endif //__INCLUDE_H__
