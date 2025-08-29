/*
 * NeuroOS-P Scheduler
 * @author Gemini
 */

#ifndef _NEUROOS_P_SCHEDULER_H
#define _NEUROOS_P_SCHEDULER_H

/**
 * @brief 스케줄러를 초기화합니다.
 */
void scheduler_init(void);

/**
 * @brief 스케줄러를 실행하여, 다음에 실행할 태스크를 찾아 문맥을 교환합니다.
 * 
 * 현재는 가장 간단한 라운드-로빈 방식으로 다음 태스크를 찾습니다.
 */
void schedule(void);


#endif // _NEUROOS_P_SCHEDULER_H
