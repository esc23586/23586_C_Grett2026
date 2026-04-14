/*
 * PWM1.h
 *
 * Created: 14/04/2026 12:13:56
 *  Author: grett
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <stdint.h>

// Inicializa el PWM usando Timer1 en PB1 (OC1A)
void PWM1_Init(void);

// Ajusta el duty cycle del PWM
void PWM1_SetDuty(uint16_t duty);

#endif