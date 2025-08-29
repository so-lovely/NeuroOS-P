/*
 * NeuroOS-P Event Signaling
 * @author Gemini
 */

#ifndef _NEUROOS_P_EVENT_H
#define _NEUROOS_P_EVENT_H

#include "kernel/types.h"

/**
 * @brief Wait for an event to be signaled. (Kernel-side implementation)
 *
 * This function will block the calling task until another task
 * signals the specified event.
 *
 * @param event The event ID to wait for.
 */
void sys_event_wait(event_t event);

/**
 * @brief Signal an event. (Kernel-side implementation)
 *
 * This function will wake up all tasks that are currently waiting
 * for the specified event.
 *
 * @param event The event ID to signal.
 */
void sys_event_signal(event_t event);

#endif // _NEUROOS_P_EVENT_H
