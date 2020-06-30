#include "control.h"
#include <dsp.h>
#include <stdio.h>
#include <libpic30.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "motor.h"
#include <math.h>

#define L               _Q16ftoi(0.17)          /* distance between the wheels  */
#define R               _Q16ftoi(0.03)          /* radius of the wheels         */
#define N               _Q16ftoi(48 * 20.4)     /* Counts per revolution        */

#define _2piRoverN      _Q16ftoi(0.00019249954) /* 2 * PI * R / N               */
#define _Nover2pi       _Q16ftoi(155.844520276) /* N / (2 * PI)                 */

#define _pi_q16         _Q16ftoi(PI)            /* PI                           */
#define _2pi_q16        _Q16ftoi(2 * PI)        /* 2 * PI                       */

#define MAX_VEL         60                      /* Maximum velocity for a wheel */

volatile tPID leftPID, rightPID;

volatile fractional abcCoefficientLeft[3] __attribute__ ((section (".xbss, bss, xmemory")));
volatile fractional controlHistoryLeft[3] __attribute__ ((section (".ybss, bss, ymemory")));
volatile fractional kCoeffsLeft[] = {0,0,0};

volatile fractional abcCoefficientRight[3] __attribute__ ((section (".xbss, bss, xmemory")));
volatile fractional controlHistoryRight[3] __attribute__ ((section (".ybss, bss, ymemory")));
volatile fractional kCoeffsRight[] = {0,0,0};

volatile _Q16 vr, vl;
volatile _Q16 x, y, fi;

void CONTROL_init(void)
{
    MOTOR_rightInit();
    MOTOR_leftInit();
}

void CONTROL_start(void)
{
    MOTOR_rightEnable();
    MOTOR_leftEnable();
}

void CONTROL_stop(void)
{
    MOTOR_rightDisable();
    MOTOR_leftDisable();
}

int32_t CONTROL_xGet(void)
{
    return x;
}

int32_t CONTROL_yGet(void)
{
    return y;
}

int32_t CONTROL_fiGet(void)
{
    return fi;
}

int32_t CONTROL_vlGet(void)
{
    return vl;
}

int32_t CONTROL_vrGet(void)
{
    return vr;
}

void CONTROL_vlSet(_Q16 new_vl)
{
    vl = new_vl;
}

void CONTROL_vrSet(_Q16 new_vr)
{
    vr = new_vr;
}

void CONTROL_setLeftPidCoeffs(fractional Kp, fractional Ki, fractional Kd)
{
    kCoeffsLeft[0] = Kp;
    kCoeffsLeft[1] = Ki;
    kCoeffsLeft[2] = Kd;
}

void CONTROL_setRightPidCoeffs(fractional Kp, fractional Ki, fractional Kd)
{
    kCoeffsRight[0] = Kp;
    kCoeffsRight[1] = Ki;
    kCoeffsRight[2] = Kd;
}

void CONTROL_setupPID(void)
{
    /* Set up pointer to derived coefficients */
    rightPID.abcCoefficients = &abcCoefficientRight[0];
    leftPID.abcCoefficients = &abcCoefficientLeft[0];
    
    
    /* Set up pointer to controller history samples */
    rightPID.controlHistory = &controlHistoryRight[0];
    leftPID.controlHistory = &controlHistoryLeft[0];
    
    /* Clear the controller history and the controller output */
    PIDInit(&rightPID);
    PIDInit(&leftPID);

    /* Derive the a,b, & c coefficients from the Kp, Ki & Kd */
    CONTROL_setRightPidCoeffs(Q15(0.6), Q15(0.15), Q15(0.0));
    CONTROL_setLeftPidCoeffs(Q15(0.6), Q15(0.15), Q15(0.0));
    
    PIDCoeffCalc(&kCoeffsRight[0], &rightPID);
    PIDCoeffCalc(&kCoeffsLeft[0], &leftPID);   
}

_Q16 angle_norm(_Q16 angle){
	_Q16 new_angle = angle;
    
    _Q16 k = abs(_Q16div(new_angle, _2pi_q16));
    k = k >> 16;
    k = k << 16;
    
    if(new_angle <= 0) {
        k = -k;
    }
        
    new_angle = new_angle - _Q16mpy(k, _2pi_q16);
    if(new_angle < -_pi_q16){
        new_angle = new_angle + _2pi_q16;
    }
    if(new_angle > _pi_q16){
        new_angle = new_angle - _2pi_q16;
    }
    
    return new_angle;
}

void CONTROL_updatePose(_Q16 vel_right, _Q16 vel_left)
{
    /* The result is multiplied with the sign of the velocity */
    _Q16 Dr = _Q16mpy(_2piRoverN, vel_right);
    _Q16 Dl = _Q16mpy(_2piRoverN, vel_left);
    
    _Q16 Dc = _Q16div(Dr + Dl, _Q16ftoi(2.0));
    
    x = x + _Q16mpy(Dc, _Q16cos(fi));
    y = y + _Q16mpy(Dc, _Q16sin(fi));
    fi = fi + _Q16div(Dr - Dl, L);
    
    fi = angle_norm(fi);
}

void CONTROL_runPID(void)
{
    /* Setting control reference */
    rightPID.controlReference = Q15(_itofQ16(_Q16div(vr, _Q16ftoi(MAX_VEL))));
    leftPID.controlReference = Q15(_itofQ16(_Q16div(vl, _Q16ftoi(MAX_VEL))));
    
    /* Running PID */
    PID(&rightPID);
    PID(&leftPID);
    
    /* Reading the current velocity */
    int16_t vel2 = VEL2CNT;
    int16_t vel1 = VEL1CNT;
    
    _Q16 vel2_Q16 = _Q16ftoi(abs(vel2));
    _Q16 vel1_Q16 = _Q16ftoi(abs(vel1));
    
    /* Setting the measured output to current velocity */
    rightPID.measuredOutput = Q15(_itofQ16(_Q16div(vel2_Q16, _Q16ftoi(MAX_VEL))));
    leftPID.measuredOutput = Q15(_itofQ16(_Q16div(vel1_Q16, _Q16ftoi(MAX_VEL))));
    
    /* Scaling the control output */
    uint16_t result2 = _itofQ16(_Q16mpy(_Q16ftoi(Fract2Float(rightPID.controlOutput)), _Q16ftoi(0xDB7)));
    uint16_t result1 = _itofQ16(_Q16mpy(_Q16ftoi(Fract2Float(leftPID.controlOutput)), _Q16ftoi(0xDB7)));
    
    /* Setting the new duty cycle */
    PWM_DutyCycleSet(PWM_GENERATOR_4, result2);
    PWM_DutyCycleSet(PWM_GENERATOR_6, result1);
    
    /* Updating robot POSE */
    CONTROL_updatePose(_Q16ftoi(-vel2), _Q16ftoi(vel1));
}

void CONTROL_updateWheelsSpeed(_Q16 v, _Q16 w)
{
    _Q16 VMAX = _Q16ftoi(15);
    _Q16 rotation = _Q16mpy(w, _Q16div(L, R));
    _Q16 translation = _Q16div(v, R);
    
    _Q16 aux_vr = _Q16mpy(translation + rotation, _Nover2pi);
    _Q16 aux_vl = _Q16mpy(translation - rotation, _Nover2pi);
    
    if(aux_vl > VMAX)
    {
        aux_vr = _Q16div(_Q16mpy(aux_vr, VMAX), aux_vl);
        aux_vl = VMAX;
    }
    
    if(aux_vr > VMAX)
    {
        aux_vl = _Q16div(_Q16mpy(aux_vl, VMAX), aux_vr);
        aux_vr = VMAX;
    }
    
    IFS6bits.PWM4IF = false;
    IFS6bits.PWM6IF = false;
    
    IEC6bits.PWM4IE = false;
    IEC6bits.PWM6IE = false;
    
    
    //IEC0bits.U1RXIE = 0;
    
    CONTROL_clearHistory();
    
    if(aux_vr > 0)
    {
        MOTOR_rightForward();
        vr = aux_vr;
    }
    else
    {
        MOTOR_rightBackward();
        vr = -aux_vr;
    }
    
    if(aux_vl > 0)
    {
        MOTOR_leftForward();
        vl = aux_vl;
    }
    else
    {
        MOTOR_leftBackward();
        vl = -aux_vl;
    }
    
    IFS6bits.PWM4IF = true;
    IFS6bits.PWM6IF = true;
    
    IEC6bits.PWM4IE = true;
    IEC6bits.PWM6IE = true;
    
    //IEC0bits.U1RXIE = 1;
}

void CONTROL_clearHistory(void) {
    leftPID.controlHistory[0] = 0;
    leftPID.controlHistory[1] = 0;
    leftPID.controlHistory[2] = 0;
    rightPID.controlHistory[0] = 0;
    rightPID.controlHistory[1] = 0;
    rightPID.controlHistory[2] = 0;
}