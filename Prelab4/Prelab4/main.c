/*
 * Name: Prelab4
 *
 * Created: 7/04/2026 2:00am
 * Author: Esc- 23586
 * Description:Este prelaboratorio tiene como funcionalidad, realizar un contador binario de 8 bits. 
 Este debe decrementar e incrementar con ayuda de 2 pusbottons con antirrebote en puerto C. 
 
 Debido a que más adelante se trabajará con la funcionalidad del ADC- la cual se encuentra en puerto C-
 Entonces se buscará Colocar el display en puerto D multiplexando, y en puerto B el contador binario.
  
 */

//Bloque de funcionamientos:
/*
Contador: Basicamente se utiliza puerto B y PC0 Y PC1 para el contador. 
siendo pc0 y pc1 como los bits más significativos:


*/

/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
//#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
/****************************************/
// Function prototypes
/****************************************/
// Main Function

//==============Parte del prelaboratorio:========================

int main(void)
{
	//******** Configuración salidas***********
	DDRB = 0x3F;   // PB0–PB5
	DDRC |= 0x03;  // PC0–PC1

	//********* Configuración entradas**************
	DDRC &= ~((1<<PC2) | (1<<PC3)); // PC2, PC3 entradas
	PORTC |= (1<<PC2) | (1<<PC3);   // Pull-ups

	uint8_t contador = 0; //igualo mi contador a 0 

	while (1)
	{
		// BOTÓN UP (PC2)
		if (!(PINC & (1<<PC2)))  // Presionado (activo en 0)
		{
			_delay_ms(20); // antirrebote

			if (!(PINC & (1<<PC2))) // confirmar
			{
				contador++; // overflow automático

				// Esperar a que se suelte
				while (!(PINC & (1<<PC2)));
			}
		}

		// BOTÓN DOWN (PC3)
		if (!(PINC & (1<<PC3)))
		{
			_delay_ms(20);

			if (!(PINC & (1<<PC3)))
			{
				contador--; // underflow automático

				while (!(PINC & (1<<PC3)));
			}
		}

		//  Mostrar en LEDs--

		// Bits 0–5 ? PORTB
		PORTB = contador & 0x3F;

		// Bits 6–7 ? PC0–PC1
		PORTC = (PORTC & 0xFC) | ((contador >> 6) & 0x03);
		//  ------
		
		
		//adc:
		ADC_init();
		uint16_t valor;
		while (1)
		{
			valor = ADC_read(6); // Saca el valor de mi A6 que está en la parte del potenciometro.
		}
	}
}

//===========Parte del Laboratorio: lectura ADC=====================
/*Esto significa que el ADC asume que 5 V equivalen a 1023, 
y cualquier valor inferior a 5 V será una relación entre 5 V y 1023.
*/
void ADC_init()
{
	ADMUX = (1<<REFS0); // referencia AVcc
	ADCSRA = (1<<ADEN)  // habilitar ADC
	| (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // prescaler 128 EN Teoría da 5 ms
	
}

uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);

	ADCSRA |= (1<<ADSC); // iniciar conversión

	while (ADCSRA & (1<<ADSC)); // Esperar

	return ADC;
}


/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines
