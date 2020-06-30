#include "qei.h"

void QEI_Initialize(void)
{
    POS1HLD = 0;
    POS1CNTL = 0;
    
    QEI1CON = 0x8000; //Enable QEI1
    QEI1IOCbits.FLTREN = 1;
    
    POS2HLD = 0;
    POS2CNTL = 0;
    
    QEI2CON = 0x8000; //Enable QEI1
    QEI2IOCbits.FLTREN = 1;
}
