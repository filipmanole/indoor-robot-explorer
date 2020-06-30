/**
  PWM Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    pwm.c

  @Summary
    This is the generated driver implementation file for the PWM driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for PWM.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  dsPIC33EP128GM304      
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include "pwm.h"
#include "pin_manager.h"

/**
  Section: Driver Interface
*/

void (*PWM6_ExternalCallback)(void);
void (*PWM4_ExternalCallback)(void);

void PWM6_SetExternalCallback(void (* ExternalCallback)(void))
{
    PWM6_ExternalCallback = ExternalCallback;
}

void PWM4_SetExternalCallback(void (* ExternalCallback)(void))
{
    PWM4_ExternalCallback = ExternalCallback;
}

void PWM_Initialize (void)
{
    // PCLKDIV 1; 
    PTCON2 = 0x00;
    // SYNCOEN disabled; SEIEN disabled; SESTAT disabled; SEVTPS 1; SYNCSRC SYNCI1; SYNCEN disabled; EIPU disabled; SYNCPOL disabled; 
    STCON = 0x00;
    // PCLKDIV 1; 
    STCON2 = 0x00;
    // STPER 65528; 
    STPER = 0xFFF8;
    // SSEVTCMP 0; 
    SSEVTCMP = 0x00;
    // PTPER 3342; 
    PTPER = 0xD0E;
    // SEVTCMP 1706; 
    SEVTCMP = 0x3550;
    // MDC 65535; 
    MDC = 0xFFFF;
    // CHOPCLK 0; CHPCLKEN disabled; 
    CHOP = 0x00;
    // PWMKEY 0; 
    PWMKEY = 0x00;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN disabled; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON1 = 0x00;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN disabled; XPRES disabled; ITB Primary; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON2 = 0x200;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN disabled; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON3 = 0x00;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN enabled; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON4 = 0x400;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN disabled; XPRES disabled; ITB Primary; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON5 = 0x200;
    // MDCS Primary; FLTIEN disabled; CAM Edge Aligned; DTC Positive dead time for all Output modes; TRGIEN enabled; XPRES disabled; ITB Master; IUE disabled; CLIEN disabled; MTBS disabled; DTCP disabled; 
    PWMCON6 = 0x400;
    //FLTDAT PWM1L Low, PWM1H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM1L Low, PWM1H Low; OVRDAT PWM1L Low, PWM1H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON1, 0x00, &PWMKEY);
    //FLTDAT PWM2L Low, PWM2H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM2L Low, PWM2H Low; OVRDAT PWM2L Low, PWM2H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON2, 0x00, &PWMKEY);
    //FLTDAT PWM3L Low, PWM3H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM3L Low, PWM3H Low; OVRDAT PWM3L Low, PWM3H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON3, 0x00, &PWMKEY);
    //FLTDAT PWM4L Low, PWM4H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM4L Low, PWM4H Low; OVRDAT PWM4L Low, PWM4H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON4, 0x8000, &PWMKEY);
    //FLTDAT PWM5L Low, PWM5H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH disabled; CLDAT PWM5L Low, PWM5H Low; OVRDAT PWM5L Low, PWM5H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON5, 0x00, &PWMKEY);
    //FLTDAT PWM6L Low, PWM6H Low; SWAP disabled; OVRENH disabled; PENL disabled; PMOD Complementary Output Mode; OVRENL disabled; OSYNC disabled; POLL disabled; PENH enabled; CLDAT PWM6L Low, PWM6H Low; OVRDAT PWM6L Low, PWM6H Low; POLH disabled; 
    __builtin_write_PWMSFR(&IOCON6, 0x8000, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON1, 0xFB, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON2, 0xFB, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON3, 0xFB, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON4, 0xFB, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON5, 0xFB, &PWMKEY);
    //FLTPOL disabled; CLPOL disabled; CLSRC FLT1; CLMOD disabled; FLTMOD Fault input is disabled; IFLTMOD disabled; FLTSRC FLT32; 
    __builtin_write_PWMSFR(&FCLCON6, 0xFB, &PWMKEY);
    // PDC1 1751; 
    PDC1 = 0x6D7;
    // PDC2 255; 
    PDC2 = 0xFF;
    // PDC3 0; 
    PDC3 = 0x00;
    // PDC4 1751; 
    PDC4 = 0x6D7;
    // PDC5 1751; 
    PDC5 = 0x6D7;
    // PDC6 1751; 
    PDC6 = 0x6D7;
    // PHASE1 3502; 
    PHASE1 = 0xDAE;
    // PHASE2 0; 
    PHASE2 = 0x00;
    // PHASE3 0; 
    PHASE3 = 0x00;
    // PHASE4 3342; 
    PHASE4 = 0xD0E;
    // PHASE5 3502; 
    PHASE5 = 0xDAE;
    // PHASE6 3342; 
    PHASE6 = 0xD0E;
    // DTR1 0; 
    DTR1 = 0x00;
    // DTR2 0; 
    DTR2 = 0x00;
    // DTR3 0; 
    DTR3 = 0x00;
    // DTR4 0; 
    DTR4 = 0x00;
    // DTR5 0; 
    DTR5 = 0x00;
    // DTR6 0; 
    DTR6 = 0x00;
    // ALTDTR1 0; 
    ALTDTR1 = 0x00;
    // ALTDTR2 0; 
    ALTDTR2 = 0x00;
    // ALTDTR3 0; 
    ALTDTR3 = 0x00;
    // ALTDTR4 0; 
    ALTDTR4 = 0x00;
    // ALTDTR5 0; 
    ALTDTR5 = 0x00;
    // ALTDTR6 0; 
    ALTDTR6 = 0x00;
    // SDC1 0; 
    SDC1 = 0x00;
    // SDC2 0; 
    SDC2 = 0x00;
    // SDC3 0; 
    SDC3 = 0x00;
    // SDC4 0; 
    SDC4 = 0x00;
    // SDC5 0; 
    SDC5 = 0x00;
    // SDC6 0; 
    SDC6 = 0x00;
    // SPHASE1 0; 
    SPHASE1 = 0x00;
    // SPHASE2 0; 
    SPHASE2 = 0x00;
    // SPHASE3 0; 
    SPHASE3 = 0x00;
    // SPHASE4 0; 
    SPHASE4 = 0x00;
    // SPHASE5 0; 
    SPHASE5 = 0x00;
    // SPHASE6 0; 
    SPHASE6 = 0x00;
    // TRGCMP 1750; 
    TRIG1 = 0x36B0;
    // TRGCMP 0; 
    TRIG2 = 0x00;
    // TRGCMP 0; 
    TRIG3 = 0x00;
    // TRGCMP 3342; 
    TRIG4 = 0xD0E;
    // TRGCMP 1750; 
    TRIG5 = 0x36B0;
    // TRGCMP 3342; 
    TRIG6 = 0xD0E;
    // TRGDIV 16; TRGSTRT 0; 
    TRGCON1 = 0xF000;
    // TRGDIV 1; TRGSTRT 0; 
    TRGCON2 = 0x00;
    // TRGDIV 1; TRGSTRT 0; 
    TRGCON3 = 0x00;
    // TRGDIV 10; TRGSTRT 0; 
    TRGCON4 = 0x9000;
    // TRGDIV 16; TRGSTRT 0; 
    TRGCON5 = 0xF000;
    // TRGDIV 10; TRGSTRT 0; 
    TRGCON6 = 0x9000;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON1 = 0x00;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON2 = 0x00;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON3 = 0x00;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON4 = 0x00;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON5 = 0x00;
    // BPLL disabled; BPHH disabled; BPLH disabled; BCH disabled; FLTLEBEN disabled; PLR disabled; CLLEBEN disabled; BCL disabled; PLF disabled; PHR disabled; BPHL disabled; PHF disabled; 
    LEBCON6 = 0x00;
    // LEB 0; 
    LEBDLY1 = 0x00;
    // LEB 0; 
    LEBDLY2 = 0x00;
    // LEB 0; 
    LEBDLY3 = 0x00;
    // LEB 0; 
    LEBDLY4 = 0x00;
    // LEB 0; 
    LEBDLY5 = 0x00;
    // LEB 0; 
    LEBDLY6 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON1 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON2 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON3 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON4 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON5 = 0x00;
    // CHOPLEN disabled; CHOPHEN disabled; BLANKSEL No state blanking; CHOPSEL No state blanking; 
    AUXCON6 = 0x00;
    
    //Interrupts
    IFS6bits.PWM4IF = false;
    IEC6bits.PWM4IE = true;
    //Interrupts
    IFS6bits.PWM6IF = false;
    IEC6bits.PWM6IE = true;

    // SYNCOEN enabled; SEIEN disabled; SESTAT disabled; SEVTPS 10; SYNCSRC SYNCI1; SYNCEN disabled; PTSIDL disabled; PTEN enabled; EIPU disabled; SYNCPOL disabled; 
    PTCON = 0x8109;
}

void __attribute__ ((weak)) PWM_SpecialEvent_CallBack(void)
{
    // Add your custom callback code here
}

void PWM_SpecialEvent_Tasks ( void )
{
	if(IFS3bits.PSEMIF)
	{
		// PWM SpecialEvent callback function 
		PWM_SpecialEvent_CallBack();
		
		// clear the PWM SpecialEvent interrupt flag
		IFS3bits.PSEMIF = 0;
	}
}

void __attribute__ ((weak)) PWM_Generator1_CallBack(void)
{
    // Add your custom callback code here
}

void PWM_Generator1_Tasks ( void )
{
	if(IFS3bits.PSEMIF)
	{
		// PWM Generator1 callback function 
		PWM_Generator1_CallBack();
		
		// clear the PWM Generator1 interrupt flag
		IFS5bits.PWM1IF = 0;
	}
}
void __attribute__ ((weak)) PWM_Generator2_CallBack(void)
{
    // Add your custom callback code here
}

void PWM_Generator2_Tasks ( void )
{
	if(IFS3bits.PSEMIF)
	{
		// PWM Generator2 callback function 
		PWM_Generator2_CallBack();
		
		// clear the PWM Generator2 interrupt flag
		IFS5bits.PWM2IF = 0;
	}
}
void __attribute__ ((weak)) PWM_Generator3_CallBack(void)
{
    // Add your custom callback code here
}

void PWM_Generator3_Tasks ( void )
{
	if(IFS3bits.PSEMIF)
	{
		// PWM Generator3 callback function 
		PWM_Generator3_CallBack();
		
		// clear the PWM Generator3 interrupt flag
		IFS6bits.PWM3IF = 0;
	}
}
void __attribute__ ((weak)) PWM_Generator4_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _PWM4Interrupt (  )
{
	// PWM Generator4 callback function 
	static uint8_t count4 = 0;
    
    if(count4 == 20)
    {
        if(PWM4_ExternalCallback){
            PWM4_ExternalCallback();
        }
        count4 = 0;
    }
	++count4;
	
	IFS6bits.PWM4IF = false; 
}
void __attribute__ ((weak)) PWM_Generator5_CallBack(void)
{
    // Add your custom callback code here
}

void PWM_Generator5_Tasks ( void )
{
	if(IFS3bits.PSEMIF)
	{
		// PWM Generator5 callback function 
		PWM_Generator5_CallBack();
		
		// clear the PWM Generator5 interrupt flag
		IFS6bits.PWM5IF = 0;
	}
}
void __attribute__ ((weak)) PWM_Generator6_CallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _PWM6Interrupt (  )
{
	// PWM Generator6 callback function 
		PWM_Generator6_CallBack();
    
	IFS6bits.PWM6IF = false; 
        }
	
/**
 End of File
*/
