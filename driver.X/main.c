#include <stdio.h>
#include <string.h>
#include <libq.h>

#include <p33EP128GM304.h>
#define    FCY    35020800UL    // Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>        // __delayXXX() functions macros defined here

#include "mcc_generated_files/mcc.h"
#include "control.h"
#include "motor.h"

#define MY_BUFFER_SIZE  12

#define ACK             0x4D

uint8_t received_byte;

void STATE_init(void) {
    CONTROL_init();
    PWM_ModuleDisable();
    CONTROL_stop();
    CONTROL_setupPID();
    PWM4_SetExternalCallback(CONTROL_runPID);
}

void COMMAND_sendACK() {
    //UART1_Write(ACK);
    uint8_t ack = ACK;
    UART1_mySendBuffer(&ack, sizeof (ack));
}

bool COMMAND_recvACK(void) {
    uint8_t ack = 0;
    UART1_myRecvBuffer(&ack, sizeof (ack));
    
    if(ack == ACK) {
        return true;
    }
    
    return false;
}

void COMMAND_start(void) {
    CONTROL_start();
    CONTROL_clearHistory();
    PWM_ModuleEnable();
    
    /* Send ACK */
    COMMAND_sendACK();
}

void COMMAND_stop(void) {
    PWM_ModuleDisable();
    CONTROL_stop();
    
    /* Send ACK */
    COMMAND_sendACK();
}

void COMMAND_run(void) {
    int32_t received_v = 0;
    int32_t received_w = 0;

    uint8_t *recv_buff_v = &received_v;
    uint8_t *recv_buff_w = &received_w;

    UART1_myRecvBuffer(recv_buff_v, sizeof (received_v));
    UART1_myRecvBuffer(recv_buff_w, sizeof (received_w));

    
    //CONTROL_clearHistory();
    
    CONTROL_updateWheelsSpeed(received_v, received_w);
    
    /* Send ACK */
    COMMAND_sendACK();
}

void COMMAND_pose(void) {
    int32_t x = CONTROL_xGet();
    int32_t y = CONTROL_yGet();
    int32_t fi = CONTROL_fiGet();

    uint8_t *send_x = &x;
    uint8_t *send_y = &y;
    uint8_t *send_fi = &fi;
    
    UART1_mySendBuffer(send_x, sizeof (x));
    UART1_mySendBuffer(send_y, sizeof (y));
    UART1_mySendBuffer(send_fi, sizeof (fi));

    /* Recv ACK */
    COMMAND_recvACK();
}

void COMMAND_vel(void) {
    int32_t vr = CONTROL_vrGet();
    int32_t vl = CONTROL_vlGet();

    uint8_t *send_vr = &vr;
    uint8_t *send_vl = &vl;

    UART1_mySendBuffer(send_vr, sizeof (vr));
    UART1_mySendBuffer(send_vl, sizeof (vl));
    
    /* Recv ACK */
    COMMAND_recvACK();
}

void COMMAND_recv_vel(void) {
    int32_t received_vr = 0;
    int32_t received_vl = 0;

    uint8_t *recv_buff_vr = &received_vr;
    uint8_t *recv_buff_vl = &received_vl;

    UART1_myRecvBuffer(recv_buff_vr, sizeof (received_vr));
    UART1_myRecvBuffer(recv_buff_vl, sizeof (received_vl));
    
    CONTROL_vrSet(received_vr);
    CONTROL_vlSet(received_vl);
    
    CONTROL_updatePose(received_vr, received_vl);
    
    /* Send ACK */
    COMMAND_sendACK();
    
//    int16_t vrf = received_vr / 65536;
//    int16_t vlf = received_vl / 65536;
//    CONTROL_updatePoseFloat(vrf, vlf);
}

void (*COMMANDS[6])() = {   
                            COMMAND_stop, 
                            COMMAND_start, 
                            COMMAND_run,
                            COMMAND_pose, 
                            COMMAND_vel,
                            COMMAND_recv_vel
                        };

int main(void) {
    
    SYSTEM_Initialize();
    STATE_init();

    while (1) {
        if (!UART1_ReceiveBufferIsEmpty()) {
            //received_byte = UART1_Read();
            UART1_myRecvBuffer(&received_byte, sizeof (received_byte));
            LED_Toggle();
            COMMANDS[received_byte]();
        }
    }

    return 1;
}

