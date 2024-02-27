/*
 * maintask.h
 *
 *  Created on: Feb 24, 2024
 *      Author: Andrew Gerber
 */

#ifndef __MAINTASK_H
#define __MAINTASK_H

#include "cmsis_os.h"

#ifdef __cplusplus
extern "C"
#endif
void main_init(osMutexId_t RXBuffLock);


#endif /* __MAINTASK_H */
