#ifndef __STATE_MACHINE_H
#define __STATE_MACHINE_H

#include "pi_controller.h"
#include "stm32f4xx_hal.h"

typedef enum {
    STATE_IDLE,
    STATE_MOVE_LINEAR,
    STATE_ROTATE,
    STATE_STOP,
    STATE_CUSTOM
} State;

void StateMachine_Init(void);
void StateMachine_SetState(State newState);
void StateMachine_Update(void);

#endif // __STATE_MACHINE_H
