#ifndef __ACTION_H__
#define __ACTION_H__

#include "main.h"
#include "usr_tim.h"
#include "servo.h"
#include "motor_ctrl.h"
#include "Delay.h"
#include "staMachine.h"
#include "jetson.h"
void Action_Catch_And_Put_Obj(uint8_t position);

uint8_t Action_Put_Obj_To_Map(void);
uint8_t Action_Put_Obj_To_Map_321(void);
uint8_t Action_Put_Obj_To_Map_312(void);
uint8_t Action_Put_Obj_To_Map_123(void);
uint8_t Action_Put_Obj_To_Map_132(void);
uint8_t Action_Put_Obj_To_Map_213(void);
uint8_t Action_Put_Obj_To_Map_231(void);
#endif
