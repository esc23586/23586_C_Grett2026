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
#include <avr/interrupt.h> //se habilita para pinchange
#define F_CPU 16000000UL
#include <util/delay.h>
/****************************************/
// Function prototypes

//para interrupciones
volatile uint8_t flag_up = 0;
volatile uint8_t flag_down = 0;

//======================Preliminar ================================
uint8_t valor_display = adc_val / 10; 
uint8_t decenas = valor_display / 10;
uint8_t unidades = valor_display % 10;

//  Tabla de 7 segmentos : 
const uint8_t tabla7seg[10] = {
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111  // 9
};

// Timer init
void timer0_init()
{
	TCCR0A = 0x00;
	TCCR0B = (1<<CS01) | (1<<CS00); // prescaler 64
}

// Debounce
void debounce_timer0()
{
	TCNT0 = 0;
	while (TCNT0 < 78);
}

// PCINT init
void pcint_init()
{
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT10) | (1<<PCINT11);
}


/****************************************/
// Main Function

//===========inicializar la parte del ADC==============
void ADC_init()
{
	ADMUX = (1<<REFS0); // Referencia AVcc (5V)

	ADCSRA = (1<<ADEN)  // Habilitar ADC
	| (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Prescaler 128
}

//===============Función para mostrar el ADC===============
/*Esto significa que el ADC asume que 5 V equivalen a 1023,
y cualquier valor inferior a 5 V será una relación entre 5 V y 1023.
lyuego se divide
*/


uint16_t ADC_read(uint8_t channel)
{
	// Mantener referencia y limpiar canal
	ADMUX = (1<<REFS0) | (channel & 0x07);

	ADCSRA |= (1<<ADSC); // iniciar conversión

	while (ADCSRA & (1<<ADSC)); // esperar

	return ADC;
}


//==============Parte del prelaboratorio: CONTADOR ========================
/*
int main(void)
{
	******** Configuración salidas***********
	DDRB = 0x3F;   // PB0–PB5
	DDRC |= 0x03;  // PC0–PC1

	********* Configuración entradas**************
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

		//  Mostrar en LEDs

		// Bits 0–5 ? PORTB
		PORTB = contador & 0x3F;

		// Bits 6–7 ? PC0–PC1
		PORTC = (PORTC & 0xFC) | ((contador >> 6) & 0x03);
		
	}
}
*/
	
	//  ISR
	ISR(PCINT1_vect)
	{
		if (!(PINC & (1<<PC2)))
		flag_up = 1;

		if (!(PINC & (1<<PC3)))
		flag_down = 1;
	}


//Parte del Main que se utiliza en el laboratorio:

int main(void)
{
	//******** Configuración salidas***********
	DDRB = 0x3F;   // PB0–PB5
	DDRC |= 0x03;  // PC0–PC1

	//********* Configuración entradas**************
	DDRC &= ~((1<<PC2) | (1<<PC3)); // PC2, PC3 entradas
	PORTC |= (1<<PC2) | (1<<PC3);   // Pull-ups
	timer0_init();
	pcint_init();
	//***************PINES Puerto D salida*********************
	DDRD = 0xFF; // segmentos salida
	
	//****************Logica de entrada para DIPSMux :3 ******************
	DDRC |= (1<<PC4) | (1<<PC5); // selección displays
	//(Se eligió estos para facilidad de cableado)
	

	ADC_init(); //  inicializar ADC (PARTE NUEVA)
	sei(); // habilitar interrupciones
	
	uint8_t contador = 0;
	uint16_t adc_val = 0; //  variable ADC
	

	while (1)
	{
		//  Leer potenciómetro movible en A6
		adc_val = ADC_read(6);
		
		
		// PARTE DEL PRELAB:
		
		//  BOTÓN UP
		if (flag_up)
		{
			debounce_timer0();

			if (!(PINC & (1<<PC2))) // confirmar
			{
				contador++;
				while (!(PINC & (1<<PC2))); // esperar release
			}

			flag_up = 0;
		}

		// DOWN
		if (flag_down)
		{
			debounce_timer0();

			if (!(PINC & (1<<PC3)))
			{
				contador--;
				while (!(PINC & (1<<PC3)));
			}

			flag_down = 0;
		}

		// LEDs
		// Bits 0–5 ? PORTB
		PORTB = contador & 0x3F;
		// Bits 6–7 ? PC0–PC1
		PORTC = (PORTC & 0xFC) | ((contador >> 6) & 0x03);
	}
}


/****************************************/
// NON-Interrupt subroutines
// La del timer para esperar

/****************************************/
// Interrupt routines

//Interrupción de pinchange

