/*
 * PWM1.h
 *
 * Created: 14/04/2026 12:13:56
 *  Author: grett
 
 Descripción para el archivo :
 Archivo donde se le indica al compilador, existen las funciones, para mi pwm
 */ 


#ifndef PWM1_H_
#define PWM1_H_

#include <stdint.h>

// Inicializa el PWM usando Timer1 en PB1 (OC1A)
void PWM1_Init(void);

// Ajusta el duty cycle del PWM
void PWM1_SetDuty(uint16_t duty); //Cambia el ancho del pulso, basicamente mueve el servo, revisar***

#endif