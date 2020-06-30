#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <arpa/inet.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define SAMPLE_RATE_K       10000
#define SAMPLE_RATE         100
#define SAMPLE_NUM          1024

#define BOUDRATE            460800

#define RECV_BUFF_SIZE      30

#define N                   979.2
#define R                   0.03

#define STOP_COMMAND        "stop\0"
#define START_COMMAND       "start\0"
#define RUN_COMMAND         "run\0"
#define POSE_COMMAND        "pose\0"
#define VEL_COMMAND         "vel\0"
#define SEND_VEL_COMMAND    "send_vel\0"

#define ACK                 0x4D

volatile int sample_index;
double samples[SAMPLE_NUM];

enum states {
    STOP,
    START,
    RUN,
    POSE,
    VEL,
    SEND_VEL
};

int strncicmp(char const *a, char const *b, size_t n) {
    for(size_t i = 0; i < n; ++i) {
        int d = tolower(a[i]) - tolower(b[i]);
        if(d != 0 || !a[i] || !b[i])
            return d;
    }

    return 0;
}

void serial_read_buffer(int fd, uint8_t *buffer, size_t size) {
    for(uint8_t k = 0; k < size; ++k) {
        buffer[k] = serialGetchar(fd);
        if(buffer[k] == -1) {
            printf("ERROR READING\n");
            exit(-1);
        }
    }
}

void serial_send_buffer(int fd, uint8_t *buffer, size_t size) {
    for(uint8_t k = 0; k < size; ++k) {
        serialPutchar(fd, buffer[k]);
    }
}

void sendACK(int fd) {
    uint8_t byte = ACK;
    serialPutchar(fd, byte);
    printf("ACK sent\n");
}

bool recvACK(int fd) {
    uint8_t byte = serialGetchar(fd);

    if(byte == ACK) {
        printf("ACK\n");
        return true;
    }

    printf("NACK\n");
    return false;
}

int main (int argc, char *argv [])
{
    
    wiringPiSetup();

    int fd = serialOpen("/dev/ttyS0", BOUDRATE);

    if(fd < 0) {
        printf("Serial Error\n");
        return 0;
    }
    
    if(argc == 1) {
        printf("Command format incorrect\n");
    }


    if(strncicmp(argv[1], STOP_COMMAND, strlen(STOP_COMMAND)) == 0) {
        serialPutchar(fd, STOP);
        if(!recvACK(fd)){
            //printf("NACK");
            return 1;
        }
    } else if(strncicmp(argv[1], START_COMMAND, strlen(START_COMMAND)) == 0) {
        serialPutchar(fd, START);
        if(!recvACK(fd)) {
            //printf("NACK");
            return 1;
        }
    } else if(strncicmp(argv[1], RUN_COMMAND, strlen(RUN_COMMAND)) == 0) {

        if(argc != 4) {
            printf("Command format incorrect\n");
            return 1;
        }

        float v = atof(argv[2]);
        float w = atof(argv[3]);
            
        /* Received v is in m/s and will be transformed in counts/10ms */

        v = v / 100;
        w = w / 100;

        int32_t send_v = v * 65536;
        int32_t send_w = w * 65536;

        uint8_t *v_buff = (uint8_t *) &send_v;
        uint8_t *w_buff = (uint8_t *) &send_w;

        serialPutchar(fd, RUN);
        serial_send_buffer(fd, v_buff, sizeof(send_v));
        serial_send_buffer(fd, w_buff, sizeof(send_w));

        if(!recvACK(fd)) {
            //printf("NACK");
            return 1;
        }
    } else if(strncicmp(argv[1], VEL_COMMAND, strlen(VEL_COMMAND)) == 0) {

        serialPutchar(fd, VEL);
            
        int32_t recv_vr = 0;
        uint8_t *recv_vr_buff = (uint8_t *) &recv_vr;
        serial_read_buffer(fd, recv_vr_buff, sizeof(recv_vr));

        int32_t recv_vl = 0;
        uint8_t *recv_vl_buff = (uint8_t *) &recv_vl;
        serial_read_buffer(fd, recv_vl_buff, sizeof(recv_vl));

        sendACK(fd);

        float vr = ((float) recv_vr) / 65536;
        float vl = ((float) recv_vl) / 65536;

        printf("vr: %f vl: %f\n", vr, vl);

    } else if(strncicmp(argv[1], POSE_COMMAND, strlen(POSE_COMMAND)) == 0) {

        serialPutchar(fd, POSE);

        int32_t recv_x = 0;
        uint8_t *recv_x_buff = (uint8_t *) &recv_x;
        serial_read_buffer(fd, recv_x_buff, sizeof(recv_x));

        int32_t recv_y = 0;
        uint8_t *recv_y_buff = (uint8_t *) &recv_y;
        serial_read_buffer(fd, recv_y_buff, sizeof(recv_y));


        int32_t recv_fi = 0;
        uint8_t *recv_fi_buff = (uint8_t *) &recv_fi;
        serial_read_buffer(fd, recv_fi_buff, sizeof(recv_fi));
        
        sendACK(fd);

        float x = ((float) recv_x) / 65536;
        float y = ((float) recv_y) / 65536;
        float fi = ((float) recv_fi) / 65536;

        printf("x: %.2f y: %.2f fi: %.2f\n", x, y, fi);
    } else if(strncicmp(argv[1], SEND_VEL_COMMAND, strlen(SEND_VEL_COMMAND)) == 0) {

        if(argc != 4) {
            printf("Command format incorrect!");
            return 1;
        }

        float vr = atof(argv[2]);
        float vl = atof(argv[3]);

        int32_t send_vr = vr * 65536;
        int32_t send_vl = vl * 65536;

        uint8_t *vr_buff = (uint8_t *) &send_vr;
        uint8_t *vl_buff = (uint8_t *) &send_vl;

        serialPutchar(fd, SEND_VEL);
        printf("%i\n", SEND_VEL);
        serial_send_buffer(fd, vr_buff, sizeof(send_vr));
        serial_send_buffer(fd, vl_buff, sizeof(send_vl));

        if(!recvACK(fd)) {
            //printf("NACK");
            return 1;
        }

    } else {
        printf("Command format incorrect\n");
    }

    serialClose(fd);

    return 0;
}
