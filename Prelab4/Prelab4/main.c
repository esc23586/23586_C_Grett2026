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
#include <avr/interrupt.h> 
#define F_CPU 16000000UL
#include <util/delay.h>
/****************************************/
// Function prototypes

//========== Variables globales====
//volatile uint8_t flag_up = 0;
//volatile uint8_t flag_down = 0;

volatile uint8_t counter = 0;
volatile uint8_t lecADC = 0;
volatile uint8_t last_inc = 1;
volatile uint8_t last_dec = 1;
/****************************************/

// Tabla 7 segmentos (cátodo común)
const uint8_t TABLITA[16] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F, // 9
	0x77, // A
	0x7C, // b
	0x39, // C
	0x5E, // d
	0x79, // E
	0x71  // F
};


/****************************************/
// Main Function


//===========inicializar la parte del ADC==============
/*Esto significa que el ADC asume que 5 V equivalen a 1023,
y cualquier valor inferior a 5 V será una relación entre 5 V y 1023.
luego se divide
*/

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

//Parte del Main que se utiliza en el laboratorio:

void setup() {
	cli();

	// ---------------------------
	// BOTONES (PC2, PC3)
	// ---------------------------
	DDRC &= ~((1 << PORTC2) | (1 << PORTC3));
	PORTC |= (1 << PORTC2) | (1 << PORTC3); // Pull-up

	// ---------------------------
	// LEDs CONTADOR
	// ---------------------------
	DDRB |= 0x3F; // PB0–PB5 salidas
	DDRC |= (1 << PORTC0) | (1 << PORTC1); // los más significativos

	// ---------------------------
	// DISPLAY
	// ---------------------------
	DDRD |= 0xFF; // PORTD completo salida
	DDRC |= (1 << PORTC4) | (1 << PORTC5) | (1 << PORTC6);

	// ---------------------------
	// ALARMA EN PD7
	// ---------------------------
	DDRD |= (1 << PORTD7);

	// ---------------------------
	// ADC (A7)
	// ---------------------------
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (7 << MUX0);
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC);

	// ---------------------------
	// TIMER2 (multiplexado)
	// ---------------------------
	TCCR2A = 0x00;
	TCCR2B = (1 << CS21); // prescaler 8
	TIMSK2 = (1 << TOIE2);
	TCNT2 = 150;

	sei();
}



// ---------------------------
// ADC ISR
// ---------------------------
ISR(ADC_vect) {
	lecADC = ADCH;
	ADCSRA |= (1 << ADSC);
}


// ---------------------------
// TIMER ISR (DISPLAYS)
// ---------------------------
ISR(TIMER2_OVF_vect) {

	static uint8_t disp = 0;

	// Apagar displays
	//PORTC &= (1 << PORTC4) | (1 << PORTC5);
	PORTC &= ~((1 << PORTC4) | (1 << PORTC5));

	switch(disp) {

		case 0: // Contador
		PORTD = (PORTD & 0x80) | (TABLITA[counter & 0x0F] & 0x7F);
		PORTC |= (1 << PORTC4);
		break;
		
		case 1: // ADC alto
		PORTD = (PORTD & 0x80) | (TABLITA[(lecADC >> 4) & 0x0F] & 0x7F);
		PORTC |= (1 << PORTC5);
		break;
/*
		case 1: // ADC bajo
		PORTD = (PORTD & 0x80) | (TABLITA[lecADC & 0x0F] & 0x7F);
		PORTC |= (1 << PORTC5);
		break;

		*/
	}

disp = (disp + 1) % 2;
TCNT2 = 150;
}

//loooooop; 
// MAIN
int main(void) {

	setup();
	
	while (1)
	{
		// ---------------------------
		// BOTÓN INCREMENTO (PC2)
		// ---------------------------
		if (!(PINC & (1 << PORTC2)) && last_inc) {
			_delay_ms(20);
			if (!(PINC & (1 << PORTC2))) {
				counter++;
				last_inc = 0;
			}
		}
		else if (PINC & (1 << PORTC2)) {
			last_inc = 1;
		}

		// ---------------------------
		// BOTÓN DECREMENTO (PC3)
		// ---------------------------
		if (!(PINC & (1 << PORTC3)) && last_dec) {
			_delay_ms(20);
			if (!(PINC & (1 << PORTC3))) {
				counter--;
				last_dec = 0;
			}
		}
		else if (PINC & (1 << PORTC3)) {
			last_dec = 1;
		}

		// ---------------------------
		// MOSTRAR CONTADOR EN LEDs
		// ---------------------------
		// PB0–PB5
		PORTB = (PORTB & 0xC0) | (counter & 0x3F);

		// PC0–PC1
		PORTC = (PORTC & 0xFC) | ((counter >> 6) & 0x03);

		// ---------------------------
		// ALARMA EN d7 comparación de ADC CON CONTADOR
		// ---------------------------
		if (lecADC > counter) {
			PORTD |= (1 << PORTD7);
			} else {
			PORTD &= ~(1 << PORTD7);
		}
	}
}

