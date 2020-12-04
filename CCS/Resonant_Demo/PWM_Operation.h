/*
 *  PWM_Operation.h
 *
 *  Fall 2020
 *  Matthew Roberts
 *  The University of Texas at Austin
 */
#ifndef PWM_OPERATION_H_
#define PWM_OPERATION_H_
#include <stdint.h>

extern void Epwm_Init();
extern void PWM_DIS();
extern void PWM_EN();
extern void PWM_SET(float duty, uint16_t prd);


#endif /* PWM_OPERATION_H_ */
