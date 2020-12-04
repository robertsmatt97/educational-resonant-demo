/*
 *  Main_Resonant.c
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */

#include "PWM_Operation.h"
#include "Init_Resonant.h"
#include "F28x_Project.h"
#include "device.h"
#include "board.h"
#include "driverlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*********************OPERATION MODE***************************/
volatile uint32_t ramp_flag = 0;                                // Set to 1 if you don't want to sweep, else set to 0
volatile uint16_t SCI_check = 0;                                // Set to 1 if you don't want to sweep, else set to 0
/**************************************************************/

/***************FREQUENCY CALIBRATION PARAMETERS***************/
#define LOWER_FREQUENCY 45000                                   // The lowest frequency you want the potentiometer to reach
#define UPPER_FREQUENCY 130000                                  // The highest frequency you want the potentiometer to reach
/**************************************************************/

/*******************SWEEPING PARAMETERS************************/
#define SWEEP_LOWER 20000                                       // The first frequency to be swept over
#define SWEEP_UPPER 210000                                      // The last frequency to be swept over

// If you are not sure what the below values should be, do not edit them.
#define TOTAL_TIME 10000                                        // The total amount of time spent sweeping, in milliseconds
#define RAMP_RESOLUTION 100                                     // The time between taking each sample, in milliseconds
/**************************************************************/

/*******************CALIBRATION VALUES*************************/
volatile const double Vin_calibrate = 1.9697;                   // Vin_calibrate = Vin_board / Vin_temp
volatile const double Vout_calibrate = 7.359;                   // Vout_calibrate = Vout_board / Vout_temp
/**************************************************************/

/***********************DO NOT EDIT****************************/
#define UPP_MULTIPLIER 1.15
float Duty = 0.5;
const double sweep_rate = (SWEEP_UPPER - SWEEP_LOWER)/(TOTAL_TIME / RAMP_RESOLUTION); // Hz/Sample
volatile double ramp_value = 0;

volatile double AdcResult[3], Adc_sweep_Result[2];
volatile double Vin, Vin_temp, Vout, Vout_temp, KnobPrd, fs_mod, fs = SWEEP_LOWER;

char data_buff[60];
uint16_t SCI_Status = 0U;
volatile uint16_t buff_length = 0;
uint16_t EndChar = '\n';
uint16_t sendChar = 'P';
uint16_t receivedChar;
uint16_t errorCount = 0;
unsigned char *msg;
/**************************************************************/

void main()
{

    // Initializing functions
    Resonant_Init();
    #ifdef AUTOBAUD
        //
        // Perform an autobaud lock.
        // SCI expects an 'a' or 'A' to lock the baud rate.
        //
        SCI_lockAutobaud(SCIA_BASE);
    #endif

    // Verify connection with MATLAB script
    while(SCI_check == 0)
    {
        // Check received character
        receivedChar = SCI_readCharBlockingFIFO(mySCI0_BASE);

        if((SCI_Status & SCI_RXSTATUS_ERROR) != 0)
        {
            ESTOP0;
        }

        // If the received character is not correct
        if(receivedChar != sendChar)
        {
            errorCount++;
        } else
        {
            SCI_disableModule(mySCI0_BASE);
            SCI_check = 1;
        }
    }

    // Enable Interrupts
    EINT;
    ERTM;

    if(ramp_flag == 0)
        {SCI_enableModule(mySCI0_BASE);}

    // Wait for data to be sent to MATLAB
    while(ramp_flag == 0)
    {
        EPwm1Regs.ETCLR.bit.INT = 1;
        EPwm1Regs.ETCLR.bit.SOCA = 1;
    }

    CpuTimer0Regs.TCR.bit.TSS = 1;                              // Stop timer

    // Send End of Transmission Character to MATLAB
    if(SCI_getRxFIFOStatus(mySCI0_BASE) == SCI_FIFO_RX0)
    {SCI_writeCharNonBlocking(mySCI0_BASE, EndChar);}
    SCI_disableModule(mySCI0_BASE);

    // Normal Operation
    for(;;)
    {
        EPwm1Regs.ETCLR.bit.INT = 1;
        EPwm1Regs.ETCLR.bit.SOCA = 1;
    }
}

__interrupt void Freq_Mod()
{

    // Enable Interrupts
    Interrupt_clearACKGroup(0xFFFFU);
    EINT;
    if(ramp_flag == 0)
    {
        // Continue sweeping
        KnobPrd = (100000000/fs) - 1;
        PWM_SET(Duty, KnobPrd);

    } else
    {
        // Any Result from the ADC will give: ADCRESULT = 4096*(ADCIN / 3.3) where ADCIN is the analog input voltage (0-3.3v)
        AdcResult[0] = AdcaResultRegs.ADCRESULT0;               // knob for switching period
        AdcResult[1] = AdccResultRegs.ADCRESULT0;               // Vin
        AdcResult[2] = AdcbResultRegs.ADCRESULT0;               // Vout

        fs_mod = (((AdcResult[0]/4096.0)*((UPPER_FREQUENCY*UPP_MULTIPLIER)-LOWER_FREQUENCY))) + LOWER_FREQUENCY;
        KnobPrd = (100000000/fs_mod) - 1;                       // / prd is the period counter value, which can be calculated as prd = 100Meg/fs.
        Vin = AdcResult[1]*(3.3/4096) * (Vin_calibrate);
        Vout = AdcResult[2]*(3.3/4096) * (Vout_calibrate);

        // Set switching frequency based on Potentiometer position
        PWM_SET(Duty, KnobPrd);

    }

    PieCtrlRegs.PIEACK.bit.ACK3 = 1;                            // Clears the group 1 PIEACK bit so that another interrupt may be handled now
    DINT;

}

__interrupt void Sweep_Routine()
{

    // Enable Interrupts
    Interrupt_clearACKGroup(0xFFFFU);
    __asm("  NOP");
    EINT;

    if(ramp_flag == 0)
    {
        // Checks for end of sweep
        if (fs >= SWEEP_UPPER)
        {
            ramp_flag = 1;
        }

        if(buff_length > 0)
        {SCI_writeCharArray(SCIA_BASE, (uint16_t*)data_buff, 21);}

        // Any Result from the ADC will give: ADCRESULT = 4096*(ADCIN / 3.3) where ADCIN is the analog input voltage (0-3.3v)
        Adc_sweep_Result[0] = AdccResultRegs.ADCRESULT0;    // Vin
        Adc_sweep_Result[1] = AdcbResultRegs.ADCRESULT0;    // Vout

        // Converting Results
        Vin_temp = Adc_sweep_Result[0]*(3.3/4096);
        Vout_temp = Adc_sweep_Result[1]*(3.3/4096);
        Vin = Adc_sweep_Result[0]*(3.3/4096) * (Vin_calibrate);
        Vout = Adc_sweep_Result[1]*(3.3/4096) * (Vout_calibrate);

        // Write Data into buffer
        buff_length = sprintf(data_buff,"%5d,%2d,%04d,%2d,%04d",(int) round(fs/100),(int) Vin, (int) ((Vin - (int) Vin ) * 10000),(int) Vout, (int) ((Vout - (int) Vout ) * 10000)); // @suppress("Invalid arguments")

        // Update fs
        ramp_value += sweep_rate;
        fs = ramp_value + SWEEP_LOWER;

        PieCtrlRegs.PIEACK.bit.ACK1 = 1;                    // Clears the group 3 PIEACK bit so that another interrupt may be handled

    } else
    {
        ;
    }
    DINT;

}

