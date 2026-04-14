/*
 * PWM2.h
 *
 * Created: 14/04/2026 14:22:14
 *  Author: grett
 Solo se declara su existencia.
 */ 


/*
#ifndef PWM2_H_
#define PWM2_H_

#include <stdint.h>

// Inicializa el PWM usando Timer1 en PB1 (OC1A)
void PWM2_Init(void);

// Ajusta el duty cycle del PWM
void PWM2_SetDuty(uint16_t duty); //Cambia el ancho del pulso, basicamente mueve el servo, revisar***

#endif
*/

#ifndef PWM2_H_
#define PWM2_H_

#include <stdint.h>
//Se eliminó Init puesto que daba problemas: 
// Ajusta el duty cycle para el segundo servo (OC1B)
void PWM2_SetDuty(uint16_t duty);

#endif

