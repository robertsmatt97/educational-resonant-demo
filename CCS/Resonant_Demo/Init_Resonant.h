/*
 *  Init_Resonant.h
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */

#ifndef INIT_RESONANT_H_
#define INIT_RESONANT_H_
#include <stdint.h>

extern void Adc_Init();
extern void GPIO_Init();
extern void Interrupt_Init();
extern void initCPUTimers(void);
extern void configCPUTimer(uint32_t cpuTimer, float freq, float period);
extern void Timer_Init();
extern void SCI_Init();
extern void Resonant_Init();


#endif /* INIT_RESONANT_H_ */
