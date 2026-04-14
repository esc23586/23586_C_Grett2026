/*
 * adc.c
 *
 * Created: 14/04/2026 12:33:16
 *  Author: grett
 */ 
#include "adc.h"
#include <avr/io.h>

void ADC_Init(void)
{
	// Referencia  para mi AVcc, osea (5V)
	ADMUX = (1 << REFS0); 

	// Habilitar mi ADC y con prescaler 128
	ADCSRA = (1 << ADEN) |
	(1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); //ADC trabaja en rango ~125 kHz
}

uint16_t ADC_Read(uint8_t channel)
{
	// Limitar canal a 0–7
	//Esta es la arte a mo dificar a futuro
	channel &= 0x07;

	// Seleccionar canal (ej: 7 = A7)
	ADMUX = (ADMUX & 0xF0) | channel;

	// Iniciar conversión
	ADCSRA |= (1 << ADSC);

	// Esperar a que termine
	while (ADCSRA & (1 << ADSC));

	return ADC;
}