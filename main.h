#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include "delay.h"
#include "chassis.h"
#include "can.h"
#include "PID.h"
#include "base.h"
#include "stdio.h"
#include "timer.h"
#include <math.h>
#include "string.h"
#include "action.h"
#include "remote_control.h"
#include "bluetooth.h"
#include "bsp.h"
#define MyAbs(x) ((x > 0) ? (x) : (-x))

void BSP_Init(void);	

#endif
