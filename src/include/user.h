/*
 * NeuroOS-P User-facing API
 * @author Gemini
 */

#ifndef _NEUROOS_P_USER_H
#define _NEUROOS_P_USER_H

#include "kernel/types.h"

// 시스템 콜 래퍼 함수들
void sys_kputs(const char *s);
void sleep(uint64_t ticks);
void event_wait(event_t event);
void event_signal(event_t event);


#endif // _NEUROOS_P_USER_H
