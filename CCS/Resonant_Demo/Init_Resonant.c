/*
 *  Init_Resonant.c
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */

#include "stdint.h"
#include "F28x_Project.h"
#include "PWM_Operation.h"
#include "ADC_Operation.h"
#include "board.h"
#include "driverlib.h"
#include "device.h"

#define RAMP_RESOLUTION 2 // ms/Sample
#define SYS_CLOCK 100000000 // Hz

extern __interrupt void Freq_Mod();
extern __interrupt void Sweep_Routine();

uint16_t cpuTimer0IntCount;

// Sets up GPIO on the board to their respective input signals
void GPIO_Init()
{

    EALLOW;

    // GPAMUX1 register, pg: 876
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;                     // Sets GPIO0 to EPWM1_A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;                     // Sets GPIO1 to EPWM_B

    // GPADIR register. pg: 878
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;                      // Sets GPIO0 as an output
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;                      // Sets GPIO1 as an output

    // GPBMUX1 register, pg: 903
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;                     // Sets GPIO35 to SCIA_RX
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;                     // Sets GPIO37 to SCIA_TX

    // GPBDIR register. pg: 907
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;                      // Sets GPIO35 as an input
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;                      // Sets GPIO37 as an output

    GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;                      // Sets GPIO35 as an input
    GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;                      // Sets GPIO37 as an output

    EDIS;

    GPIO_unlockPortConfig(GPIO_PORT_A, 0xFFFFFFFF); // @suppress("Invalid arguments")
    GPIO_unlockPortConfig(GPIO_PORT_B, 0xFFFFFFFF); // @suppress("Invalid arguments")
    GPIO_unlockPortConfig(GPIO_PORT_H, 0xFFFFFFFF); // @suppress("Invalid arguments")

    // GPIO28 is the SCI Rx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1); // @suppress("Invalid arguments")
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN); // @suppress("Invalid arguments")
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC); // @suppress("Invalid arguments")

    // GPIO29 is the SCI Tx pin.
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1); // @suppress("Invalid arguments")
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT); // @suppress("Invalid arguments")
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC); // @suppress("Invalid arguments")

}

// Initializes SCI to send data to the XDS110 Debug Probe, which will allow MATLAB to pick up data from the board.
void SCI_Init()
{

    EALLOW;

    // mySCI0 initialization
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 9600, (SCI_CONFIG_WLEN_8 |
                                                            SCI_CONFIG_STOP_ONE |
                                                            SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);
    EDIS;

}

// Initializes all ADC channels for interpreting signal data from the converter.
void Adc_Init()
{

    Adc_A_Init();
    Adc_B_Init();
    Adc_C_Init();

}

// Clears interrupt groups and enables intended interrupts
void Interrupt_Init()
{

    DINT;                                                   // Disables global interrupts

    // PIECTRL register, pg: 508
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;                      // Enables peripheral interrupts

    Interrupt_initVectorTable();                            // Sets all interrupts to be handled by the default interrupt handler

    // PIEIER registers, pgs: 510 - 545

    // 0's will disable interrupts not being used
    PieCtrlRegs.PIEIER1.all = 0x0400;                       // Enables just the 1.7 Interrupt, for TIMER0
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 1;                            // Enables just the 3.1 Interrupt, for EPWM1
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    // PIEIFR registers, pgs: 510 - 545
    // Clears all Interrupt flags, to ensure interrupts work upon reset
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;

    // PIEACK register, pg: 509
    PieCtrlRegs.PIEACK.all = 0xFFFF;                        // Clears out every PIACK bit so that interrupts can be handled

    EALLOW;
    Interrupt_register(INT_EPWM1, &Freq_Mod);
    Interrupt_enable(INT_EPWM1);
    EDIS;

}

void
initCPUTimers(void)
{

    // Initialize timer period to maximum
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);

    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);

    // Make sure timer is stopped
    CPUTimer_stopTimer(CPUTIMER0_BASE);

    // Reload all counter register with period value
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);

}

// Initializes the selected timer to the period specified by the "freq" and "period" parameters.
void
configCPUTimer(uint32_t cpuTimer, float freq, float period)
{

    uint32_t temp;

    // Initialize timer period:
    temp = (uint32_t)(freq / 1000000 * period);
    CPUTimer_setPeriod(cpuTimer, temp);

    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    CPUTimer_setPreScaler(cpuTimer, 0);


    // Initialize timer control register.
    CPUTimer_stopTimer(cpuTimer);
    CPUTimer_reloadTimerCounter(cpuTimer);
    CPUTimer_setEmulationMode(cpuTimer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT); // @suppress("Invalid arguments")
    CPUTimer_enableInterrupt(cpuTimer);

}

void Timer_Init()
{

    Interrupt_register(INT_TIMER0, &Sweep_Routine);
    initCPUTimers();
    configCPUTimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, 100000);
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_TIMER0);

}

// Initialization functions called right as the board turns on
void Resonant_Init()
{

    Device_init();                                          // Disable Watchdog timer and more
    Device_initGPIO();                                      // Disables pinlocks

    EALLOW;
    GPIO_Init();


    // PCLKCRO register, pg: 195
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;                   // Syncronizes all ePWM modules to the TBCLK
    EDIS;

    Adc_Init();
    Epwm_Init();

    Interrupt_Init();
    Timer_Init();
    SCI_Init();

    CPUTimer_startTimer(CPUTIMER0_BASE);

}
