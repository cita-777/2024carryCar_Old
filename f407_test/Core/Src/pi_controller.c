#include "pi_controller.h"

void PI_Controller_Init(PI_Controller *controller, float Kp, float Ki)
{
    controller->Kp = Kp;
    controller->Ki = Ki;
    controller->integral = 0.0f;
}

float PI_Controller_Update(PI_Controller *controller, float error)
{
    controller->integral += error;
    return controller->Kp * error + controller->Ki * controller->integral;
}
