/*
 * PWM2.c
 *
 * Created: 14/04/2026 14:21:55
 *  Author: grett
 
 Descripción: En esta parte de aui se trabaja para el segundo Servomotor pero con Timer 2. 
 Esto con  pb3 de la señal cuadrada. 
 
 */ 
void PWM2_Init(void)
{
	// PB1 (OC1A) como salida del pinservo
	DDRB |= (1 << PB2);

	// Modo Fast PWM, TOP = ICR1
	TCCR1A = (1 << COM1A1) | (1 << WGM11);//activa salida PWM en OC1A
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11); // modo Fast PWM con ICR1 y cs11 prescaler = 8

	// Frecuencia 50 Hz (20 ms)
	ICR1 = 39999;

	// Posición inicial del servo (centro)
	OCR1A = 3000;
}

void PWM2_SetDuty(uint16_t duty)
{
	OCR1A = duty;//mov, servo
}