#ifndef __STAMACHINE_H
#define __STAMACHINE_H

#include "pi.h"
#include "stm32f4xx_hal.h"

extern uint8_t ready_flag;


typedef enum
{
    STATE_IDLE,
    STATE_MOVE_LINEAR,
    STATE_ROTATE,
    STATE_STOP,
    STATE_CUSTOM
} State;

void StateMachine_Init(void);
void StateMachine_SetState(State newState);
void StateMachine_Update(void);

#endif // __STAMACHINE_H
