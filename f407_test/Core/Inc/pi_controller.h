#ifndef __PI_CONTROLLER_H
#define __PI_CONTROLLER_H

typedef struct {
    float Kp;
    float Ki;
    float integral;
} PI_Controller;

void PI_Controller_Init(PI_Controller *controller, float Kp, float Ki);
float PI_Controller_Update(PI_Controller *controller, float error);

#endif // __PI_CONTROLLER_H
