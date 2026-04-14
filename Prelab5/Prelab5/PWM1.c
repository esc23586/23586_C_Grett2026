/*
 * PWM1.c
 *
 * Created: 14/04/2026 12:15:07
 *  Author: grett
 
 Desecripción de Archivo: Basicamente implementación pwm
 */ 


#include "PWM1.h"
#include <avr/io.h>
/*
void PWM1_Init(void)
{
	// PB1 (OC1A) como salida del pinservo 
	DDRB |= (1 << PB1);

	// Modo Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << WGM11);//activa salida PWM en OC1A
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11); // modo Fast PWM con ICR1 y cs11 prescaler = 8

	// Frecuencia 50 Hz (20 ms)
	ICR1 = 39999;

	// Posición inicial del servo (centro)
	OCR1A = 3000;
}
*/



void PWM1_Init(void)
{
	// PB1 (OC1A) y PB2 (OC1B) como salida
	DDRB |= (1 << PB1) | (1 << PB2);

	// Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11);

	// 50 Hz
	ICR1 = 39999;

	// Centro inicial Para ambos
	OCR1A = 3000;
	OCR1B = 3000;
}
void PWM1_SetDuty(uint16_t duty)
{
	OCR1A = duty;//mov, servo
}