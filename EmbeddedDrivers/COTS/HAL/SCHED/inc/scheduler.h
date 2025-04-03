#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "std_types.h"
#include "systick.h"

#define MAX_NO_OF_TASKS     100

typedef void (*CB_t)(void);

typedef enum{
    SCHED_enu_OK,
    SCHED_enu_NULL_POINTER,
    SCHED_enu_PRIORITY_OCCUPIED,
    SCHED_enu_NOK
}SCHED_enuErrorStatus_t;


typedef struct{
    CB_t CallBack;
    uint32_t InitialWait;
    uint32_t CycleTime;
    uint32_t Priority;
}SCHED_stTaskCfg_t;

void SCHED_vInit(void);

SCHED_enuErrorStatus_t SCHED_enuAddRunnable(SCHED_stTaskCfg_t* Add_stTask);

void SCHED_vStart(void);

void SHED_vStop(void);


#endif