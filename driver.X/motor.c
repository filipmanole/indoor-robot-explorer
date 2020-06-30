/*
 * File:   motor.c
 * Author: filip
 *
 * Created on May 31, 2019, 1:39 PM
 */


#include "motor.h"
#include "mcc_generated_files/mcc.h"

void MOTOR_rightEnable(void)
{
    M1_EN_SetHigh();
}

void MOTOR_leftEnable(void)
{
    M2_EN_SetHigh();
}

void MOTOR_rightDisable(void)
{
    M1_EN_SetLow();
}

void MOTOR_leftDisable(void)
{
    M2_EN_SetLow();
}

void MOTOR_rightForward(void)
{
    M1_IN1_SetLow();
    M1_IN2_SetHigh();
}

void MOTOR_leftForward(void)
{
    M2_IN1_SetLow();
    M2_IN2_SetHigh();
}

void MOTOR_rightBackward(void)
{
    M1_IN1_SetHigh();
    M1_IN2_SetLow();
}

void MOTOR_leftBackward(void)
{
    M2_IN1_SetHigh();
    M2_IN2_SetLow();
}

void MOTOR_leftInit(void)
{
    M2_EN_SetHigh();
    M2_D1_SetLow();
}

void MOTOR_rightInit(void)
{
    M1_EN_SetHigh();
    M1_D1_SetLow();
}