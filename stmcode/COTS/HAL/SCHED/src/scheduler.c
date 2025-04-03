#include "scheduler.h"

#define SCHED_INIT_WAIT_STATE       0
#define SCHED_RUNNING_STATE         1

typedef struct{
    uint8_t state;
    uint32_t CurrentVal;
}SCHED_stCurrRunnableState_t;


static volatile SCHED_stCurrRunnableState_t SCHED_arrCurrentTaskState[MAX_NO_OF_TASKS] = {{0}};
//make it an array of pointers instead
static volatile SCHED_stTaskCfg_t SCHED_arrTasks[MAX_NO_OF_TASKS] = {{0}};

static void SHED_vRun(void);

void SCHED_vInit(void){

    Systick_enuInit1Ms(SYSTICK_enu_PRESCALER_1);

    Systick_enuSetCyclicFnMs(SHED_vRun,1);

}

SCHED_enuErrorStatus_t SCHED_enuAddRunnable(SCHED_stTaskCfg_t* Add_stTask){

    SCHED_enuErrorStatus_t Loc_enuStatus = SCHED_enu_OK;

    if(Add_stTask->CallBack==NULL){
        Loc_enuStatus = SCHED_enu_NULL_POINTER;
    }
    else if(SCHED_arrTasks[Add_stTask->Priority].CallBack != NULL){
        Loc_enuStatus = SCHED_enu_PRIORITY_OCCUPIED;
    }
    else{
        SCHED_arrTasks[Add_stTask->Priority].CallBack = Add_stTask->CallBack;
        SCHED_arrTasks[Add_stTask->Priority].CycleTime = Add_stTask->CycleTime;
        SCHED_arrTasks[Add_stTask->Priority].Priority = Add_stTask->Priority;
        SCHED_arrTasks[Add_stTask->Priority].InitialWait = Add_stTask->InitialWait;        
    }

    return Loc_enuStatus;
}

static void SHED_vRun(void){
    
    uint32_t i = 0;
    for(;i<MAX_NO_OF_TASKS;i++){
        if(SCHED_arrTasks[i].CallBack != NULL){
            SCHED_arrCurrentTaskState[i].CurrentVal++;
            switch(SCHED_arrCurrentTaskState[i].state){
                case SCHED_INIT_WAIT_STATE:
                if(SCHED_arrCurrentTaskState[i].CurrentVal >= SCHED_arrTasks[i].InitialWait){
                    SCHED_arrCurrentTaskState[i].CurrentVal = 0;
                    SCHED_arrCurrentTaskState[i].state = SCHED_RUNNING_STATE;
                    SCHED_arrTasks[i].CallBack();
                }
                else{
                    /*wait*/
                }
                break;

                case SCHED_RUNNING_STATE:
                if(SCHED_arrCurrentTaskState[i].CurrentVal >= SCHED_arrTasks[i].CycleTime){
                    SCHED_arrCurrentTaskState[i].CurrentVal = 0;
                    SCHED_arrTasks[i].CallBack();
                }
                else{
                    /*wait*/
                }
                break;
            
                default:
                break;
            }
        }
    }

}

void SCHED_vStart(void){
    Systick_vStart();
    while(1){
        //idle task
    }
}

void SHED_vStop(void){
    Systick_vStop();
}

#undef SCHED_INIT_WAIT_STATE       
#undef SCHED_RUNNING_STATE         