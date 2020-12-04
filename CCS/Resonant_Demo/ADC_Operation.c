/*
 *  ADC_Operation.c
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */

#include "F28x_Project.h"
#include "device.h"
#include "ADC_Operation.h"
#include "board.h"
#include "driverlib.h"

#define ADC_TRIG_SELECT 0x05
#define SAMPLE_WINDOW 25                                        // 1 us ADC Sample Window

volatile struct ADC_REGS *adc_a_module = &AdcaRegs;
volatile struct ADC_REGS *adc_b_module = &AdcbRegs;
volatile struct ADC_REGS *adc_c_module = &AdccRegs;


// This Channel, specifically at ADCINA0, will read the voltage measured at the potentiometer that is responsible for varying the switching frequency
void Adc_A_Init()
{

    EALLOW;                                                     // Unlocks Write protection for certain ADC Regs

    // ADCCTL1 register, pg: 1499; ADCCTL2 register, pg: 1501
    adc_a_module->ADCCTL1.bit.ADCPWDNZ = 1;                     // ADC-A power up
    adc_a_module->ADCCTL2.bit.PRESCALE = 6;                     // ADC Clock = SYSCLK / 4;

    // ADCSOC0CTL register, pg: 1534
    adc_a_module->ADCSOC0CTL.bit.TRIGSEL = 5;                   // SOC-0 software start for ePWM1/ADCSOCA
    adc_a_module->ADCSOC0CTL.bit.CHSEL = 0;                     // SOC-0 converts ADCINA0
    adc_a_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW-1;       // Sample window is SAMPLE_WINDOW clock cycles

    // ADCSOCPRICTL register, pg: 1513
    adc_a_module->ADCSOCPRICTL.all = 0;                         // Round robin control. Conversion starts from SOC0.

    // ADCBURSTCTL register, pg: 1502
    adc_a_module->ADCBURSTCTL.bit.BURSTEN = 0;                  // Disable burst mode

    // ADCINTFLGCLR regsiter, pg: 1506; ADCINTOVFCLR register, pg: 1508
    adc_a_module->ADCINTFLGCLR.all = 15;                        // Clears all the interrupt flags
    adc_a_module->ADCINTOVFCLR.all = 15;                        // Clears all the interrupt overflow flags

    // ADCINTSEL1N2 register, pg: 1509; ADCINTSEL3N4 register, pg: 1511
    adc_a_module->ADCINTSEL1N2.all = 0;                         // Disable Interrupts ADCINT1 and ADCINT2
    adc_a_module->ADCINTSEL3N4.all = 0;                         // Disable Interrupts ADCINT3 and ADCINT4

    // ADCINTSOCSEL1 register, pg: 1515; ADCINTSOCSEL2 register, pg: 1517
    adc_a_module->ADCINTSOCSEL1.all = 0;                        // ADC interrupt doesn't trigger any SOC
    adc_a_module->ADCINTSOCSEL2.all = 0;                        // ADC interrupt doesn't trigger any SOC

    EDIS;                                                       // Re-locks the Write Protected Registers

}

// This Channel, specifically at ADCINB6, will read the input voltage of the converter
void Adc_B_Init()
{

    EALLOW;                                                     // Unlocks Write protection for certain ADC Regs

    // ADCCTL1 register, pg: 1499; ADCCTL2 register, pg: 1501
    adc_b_module->ADCCTL1.bit.ADCPWDNZ = 1;                     // ADC-B power up
    adc_b_module->ADCCTL2.bit.PRESCALE = 6;                     // ADC Clock = SYSCLK / 4;

    // ADCSOC0CTL register, pg: 1534
    adc_b_module->ADCSOC0CTL.bit.TRIGSEL = 5;                   // SOC-0 software start for ePWM1/ADCSOCA
    adc_b_module->ADCSOC0CTL.bit.CHSEL = 6;                     // SOC-0 converts ADCINB6
    adc_b_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW-1;       // Sample window is SAMPLE_WINDOW clock cycles

    // ADCSOCPRICTL register, pg: 1513
    adc_b_module->ADCSOCPRICTL.all = 0;                         // Round robin control. Conversion starts from SOC0.

    // ADCBURSTCTL register, pg: 1502
    adc_b_module->ADCBURSTCTL.bit.BURSTEN = 0;                  // Disable burst mode

    // ADCINTFLGCLR regsiter, pg: 1506; ADCINTOVFCLR register, pg: 1508
    adc_b_module->ADCINTFLGCLR.all = 15;                        // Clears all the interrupt flags
    adc_b_module->ADCINTOVFCLR.all = 15;                        // Clears all the interrupt overflow flags

    // ADCINTSEL1N2 register, pg: 1509; ADCINTSEL3N4 register, pg: 1511
    adc_b_module->ADCINTSEL1N2.all = 0;                         // Disable Interrupts ADCINT1 and ADCINT2
    adc_b_module->ADCINTSEL3N4.all = 0;                         // Disable Interrupts ADCINT3 and ADCINT4

    // ADCINTSOCSEL1 register, pg: 1515; ADCINTSOCSEL2 register, pg: 1517
    adc_b_module->ADCINTSOCSEL1.all = 0;                        // ADC interrupt doesn't trigger any SOC
    adc_b_module->ADCINTSOCSEL2.all = 0;                        // ADC interrupt doesn't trigger any SOC

    EDIS;                                                       // Re-locks the Write Protected Registers

}

// This Channel, specifically at ADCINC5, will read the output voltage of the converter
void Adc_C_Init()
{

    EALLOW;                                                     // Unlocks Write protection for certain ADC Regs

    // ADCCTL1 register, pg: 1499; ADCCTL2 register, pg: 1501
    adc_c_module->ADCCTL1.bit.ADCPWDNZ = 1;                     // ADC-C power up
    adc_c_module->ADCCTL2.bit.PRESCALE = 6;                     // ADC Clock = SYSCLK / 4;

    // ADCSOC0CTL register, pg: 1534
    adc_c_module->ADCSOC0CTL.bit.TRIGSEL = 5;                   // SOC-0 software start for ePWM1/ADCSOCA
    adc_c_module->ADCSOC0CTL.bit.CHSEL = 5;                     // SOC-0 converts ADCINC5
    adc_c_module->ADCSOC0CTL.bit.ACQPS = SAMPLE_WINDOW-1;       // Sample window is SAMPLE_WINDOW clock cycles

    // ADCSOCPRICTL register, pg: 1513
    adc_c_module->ADCSOCPRICTL.all = 0;                         // Round robin control. Conversion starts from SOC0.

    // ADCBURSTCTL register, pg: 1502
    adc_c_module->ADCBURSTCTL.bit.BURSTEN = 0;                  // Disable burst mode

    // ADCINTFLGCLR regsiter, pg: 1506; ADCINTOVFCLR register, pg: 1508
    adc_c_module->ADCINTFLGCLR.all = 15;                        // Clears all the interrupt flags
    adc_c_module->ADCINTOVFCLR.all = 15;                        // Clears all the interrupt overflow flags

    // ADCINTSEL1N2 register, pg: 1509; ADCINTSEL3N4 register, pg: 1511
    adc_c_module->ADCINTSEL1N2.all = 0;                         // Disable Interrupts ADCINT1 and ADCINT2
    adc_c_module->ADCINTSEL3N4.all = 0;                         // Disable Interrupts ADCINT3 and ADCINT4

    // ADCINTSOCSEL1 register, pg: 1515; ADCINTSOCSEL2 register, pg: 1517
    adc_c_module->ADCINTSOCSEL1.all = 0;                        // ADC interrupt doesn't trigger any SOC
    adc_c_module->ADCINTSOCSEL2.all = 0;                        // ADC interrupt doesn't trigger any SOC

    EDIS;                                                       // Re-locks the Write Protected Registers

}
