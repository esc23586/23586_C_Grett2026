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

volatile uint8_t display_toggle = 0;
volatile uint8_t high_global = 0;
volatile uint8_t low_global = 0;
/****************************************/

//  Tabla de 7 segmentos : 
const uint8_t tabla7seg[16] = {
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

// //========= TIMER--Debounce ==========

void timer0_init()
{
	TCCR0A = 0x00;
	TCCR0B = (1<<CS01) | (1<<CS00); // prescaler 64
	TIMSK0 = (1<<TOIE0); // habilitar overflow interrupt
}

// Debounce
void debounce_timer0()
{
	TCNT0 = 0;
	while (TCNT0 < 78);
}

// ======= PCINT =======
void pcint_init()
{
	
	PCICR |= (1<<PCIE1); //port C
	PCMSK1 |= (1<<PCINT10) | (1<<PCINT11); //para mis botones pc2 y pc3
}

//==================== ADC ====================
void ADC_init()
{
	ADMUX = (1<<REFS0); // AVcc

	ADCSRA = (1<<ADEN) |
	(1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // prescaler 128
}

uint16_t ADC_read(uint8_t channel)
{
	ADMUX = (1<<REFS0) | (channel & 0x07);

	ADCSRA |= (1<<ADSC);

	while (ADCSRA & (1<<ADSC));

	return ADC;
}

//==================== MULTIPLEX ====================
void multiplexar(uint8_t high, uint8_t low)
{
	// Display alto
	PORTC &= 0b11001111; // limpiar PC4 y PC5
	PORTD = tabla7seg[high];
	PORTC |= (1<<PC4);
	_delay_ms(5);

	// Display bajo
	PORTC &= 0b11001111;
	PORTD = tabla7seg[low];
	PORTC |= (1<<PC5);
	_delay_ms(5);
}

//==================== ISR ====================

ISR(TIMER0_OVF_vect)
{
	if (display_toggle == 0)
	{
		PORTC &= 0b11001111;
		PORTD = tabla7seg[high_global];
		PORTC |= (1<<PC4);
		display_toggle = 1;
	}
	else
	{
		PORTC &= 0b11001111;
		PORTD = tabla7seg[low_global];
		PORTC |= (1<<PC5);
		display_toggle = 0;
	}
}

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

int main(void)
{
	//******** Configuración salidas***********
	DDRB = 0x3F;   // PB0–PB5
	DDRC |= 0x03;  // PC0–PC1
	DDRD = 0xFF;    // 7  segmentos
	
	
	//********* Configuración entradas**************
	DDRC &= ~((1<<PC2) | (1<<PC3)); // PC2, PC3 entradas
	PORTC |= (1<<PC2) | (1<<PC3);   // Pull-ups
	
	//****************Logica de entrada para DIPSMux :3 ******************
	DDRC |= (1<<PC4) | (1<<PC5); // selección displays
	//(Se eligió estos para facilidad de cableado)
	
	//***********INITs*************
	timer0_init();
	pcint_init();
	ADC_init();
	sei();
	
	//========== VARIABLES ==========
	uint8_t contador = 0;
	uint16_t adc_val = 0;


//loooooop; 

	while (1)
	{
		//  Leer potenciómetro  en A6
		adc_val = ADC_read(6); // REVISAR

		uint8_t adc_8bits = adc_val >> 2;

		high_global = (adc_8bits >> 4) & 0x0F;
		low_global  = adc_8bits & 0x0F;

		// Multiplexado
		multiplexar(high, low);
		
		
		// PARTE DEL PRELAB: contador
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
		PORTC = (PORTC & 0b11111100) | ((contador >> 6) & 0x03);
		
		
	}
}


/****************************************/
// NON-Interrupt subroutines


/****************************************/
// Interrupt routines

