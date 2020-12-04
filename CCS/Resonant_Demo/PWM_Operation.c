/*
 *  PWM_Operation.c
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */
#include "F28x_Project.h"
#include "device.h"
#include "PWM_Operation.h"
#include "board.h"
#include "driverlib.h"

#define PWM_DB  1                                           // Time where switches are both off; Allows for ZVS
                                                            // 1 = 0.01 us (microseconds)

volatile struct EPWM_REGS* PWM_MOD = &EPwm1Regs;

void Epwm_Init()
{

    GPIO_setPadConfig(0, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_0_EPWM1_A);

    PWM_MOD->TBCTL.bit.PRDLD = TB_IMMEDIATE;                // set Immediate load

    // TBPRD register, pg: 1938
    PWM_MOD->TBPRD = 4999;                                  // 20 kHZ switching frequency
    float Ini_prd = PWM_MOD->TBPRD;

    // CMPA register, pg: 1939; CMPB register, pg: 1940
    PWM_MOD->CMPA.bit.CMPA = Ini_prd/2;                     // 50% duty cycle
    PWM_MOD->CMPB.bit.CMPB = Ini_prd/4;                     // Counter that triggers ADC SOC

    // TBCTR register, pg: 1891
    PWM_MOD->TBCTR = 0x0000;                                // Initializes the 16-bit counter to 0

    // TBCTL register, pg: 1888
    PWM_MOD->TBCTL.bit.CTRMODE = 0;                         // Up-count Mode
    PWM_MOD->TBCTL.bit.SYNCOSEL = 0;                        // Syncing is based off of EPWMxSYNCI or SWFSYNC (Software Forced Sync)
    PWM_MOD->TBCTL.bit.PHSEN = 0;                           // Phase Register doesn't create the counters
    PWM_MOD->TBCTL.bit.HSPCLKDIV = TB_DIV1;                 // TBCLK = EPWMCLK
    PWM_MOD->TBCTL.bit.CLKDIV = TB_DIV1;                    // TBCLK = EPWMCLK

    // CMPCTL register, pg: 1893
    PWM_MOD->CMPCTL.bit.SHDWAMODE = CC_IMMEDIATE;           // Transfers shadow register data to active register
    PWM_MOD->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;            // Every transfer occurs at CTR = 0
    PWM_MOD->CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;           // Transfers shadow register data to active register
    PWM_MOD->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;            // Every transfer occurs at CTR = 0

    // DBCTL register, pg: 1897
    PWM_MOD->DBCTL.bit.IN_MODE = 0;                         // AQ module makes the delay
    PWM_MOD->DBCTL.bit.POLSEL = 2;                          // Active High Complementary with B inversion, gives period when both switches are off.
    PWM_MOD->DBCTL.bit.OUT_MODE = 3;                        // RED and FED is enabled

    // DBRED register, pg: 1933; DBFED register, pg: 1935
    PWM_MOD->DBRED.bit.DBRED = PWM_DB;                    // Rising Edge Delay, Up to 163.84 us
    PWM_MOD->DBFED.bit.DBFED = PWM_DB;                    // Falling Edge Delay, Up to 163.84 us

    // AQCTLA register, pg: 1924; AQCTLB register, pg: 1927
    PWM_MOD->AQCTLA.bit.CAU = 1;                            // Sets the ePWM-A Output to low
    PWM_MOD->AQCTLA.bit.PRD = 2;                            // When CTR reaches PRD, sets it back to 0

    // ETSEL Register, pg: 1965
    PWM_MOD->ETSEL.bit.SOCAEN = 1;                          // Enables SOCA to trigger ADC from ePWM event
    PWM_MOD->ETSEL.bit.SOCASEL = 6;                         // Generates SOCA Pulse when CTR = CMPB or CMPD (not specified)
    PWM_MOD->ETSEL.bit.SOCASELCMP = 0;                      // Specifies CMPB for above
    PWM_MOD->ETSEL.bit.INTEN = 1;                           // Enables an interrupt based on an ePWM event
    PWM_MOD->ETSEL.bit.INTSEL = 1;                          // Specifies the interrupt to be generated when CTR = 0

    // ETPS Register, pg: 1969
    PWM_MOD->ETPS.bit.INTPRD = 1;
    PWM_MOD->ETPS.bit.SOCAPRD = 1;

    // TZCTL register, pg: 1948
    EALLOW;
    PWM_MOD->TZCTL.bit.TZA = TZ_FORCE_LO;                   // When a trip event occurs, forces EPWMA to high impedance (OFF)
    PWM_MOD->TZCTL.bit.TZB = TZ_FORCE_LO;                   // When a trip event occurs, forces EPWMB to high impedance (OFF)
    EDIS;

}

// Disable the PWM by forcing a One-Shot trip event
void PWM_DIS()
{

    // TZFRC register, pg: 1964
    EALLOW;
    PWM_MOD->TZFRC.bit.OST = 1;
    EDIS;
}

// Enable the PWM after PWM_DIS by clearing the One-Shot Trip Latch Flag
void PWM_EN()
{

    // TZCTR register, pg: 1961
    EALLOW;
    PWM_MOD->TZCLR.bit.OST = 1;
    EDIS;
}

// Function to update the PWM waveform and counters
// Duty is the duty cycle (0-1); prd is the period counter value, which can be calculated as prd = 100Meg/fs.
void PWM_SET(float duty, Uint16 prd)
{

    PWM_MOD ->TBPRD = prd;
    float cmpa = duty*prd;
    float cmpb = prd/4;
    PWM_MOD->CMPA.bit.CMPA = cmpa;
    PWM_MOD->CMPB.bit.CMPB = cmpb;

}
