/*
 * PWMLed.h
 *
 * Created: 14/04/2026 15:52:33
 *  Author: grett
 */ 


#ifndef PWMLED_H_
#define PWMLED_H_

#include <stdint.h>

// Inicializa PWM por software
void PWM3_Init(void);

// Cambia duty cycle (0–255)
void PWM3_SetDuty(uint8_t duty);

#endif /* PWMLED_H_ */