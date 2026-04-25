/*
 * PreLab6.c
 *
 * Created:23586
 * Author: Grettel Escobedo 
 * Description: 
 *Pre lab: 
 *Parte 1. Envíe un caracter desde el microcontrolador hacia la computadora y mírela en la hiperterminal.
 *Parte 2. Reciba un carácter desde la hiperterminal y muéstrelo en el puerto B.
 */


/****************************************/
// Encabezado (Libraries)
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC/adc.h"
#include <stdlib.h>   // para itoa

/****************************************/
// Function prototypes
void intUART(void);
void writeChar(char c);
void writeString(char *string);
void writeNumber(uint8_t num);
void displayASCII(uint8_t value);
void printMenu(void);


/****************************************/
// Main Function
volatile uint8_t modo = 0;


//parte del Buffer
char buffer[10];

//MAin loop:
int main(void)
{
	cli();
	//Entradas- leds:
		// LED en PB5 como salida --- va indicando el modo.
		DDRB |= (1<<DDB5);
		PORTB &= ~(1<<PORTB5); // inicialmente apagado
	
		//  ---Configurar LEDs-----
		// PD2–PD7 (6 LEDs)
		DDRD |= (1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);
		// PB0–PB1 (2 LEDs)
		DDRB |= (1<<DDB0)|(1<<DDB1);

		// Apagar todos al inicio
		PORTD &= ~((1<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));
		PORTB &= ~((1<<PORTB0)|(1<<PORTB1));
		
	//inincializaciónes
	intUART();//  UART
	ADC_Init();// la lectura adc

	sei();// (no es necesario si no usas RX, pero son buenas costumbres xd)

	//writeChar('G');  // solo una vez

	printMenu();//mi función para el menu
	while (1)
	{
	
	}
}

//para el menu 
void printMenu(void)
{
	writeString("=========== Menu =============\r\n");
	writeString("1: Leer pot \r\n");
	writeString("2: Valor en ASCII\r\n");
	writeString("==============================\r\n");
	writeString("\r\n");
	writeString("\r\n");
	PORTB &= ~(1<<PORTB5); // apagar LED
}

// Inicialización UART
void intUART(void)
{
	// RX (PD0) entrada
	DDRD &= ~(1<<DDD0);

	// TX (PD1) salida
	DDRD |= (1<<DDD1);

	UCSR0A = 0;

	
	// Habilitar RX, TX e interrupción RX
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	//si solo habilito la de tx, solo se manda, si habilito el rx me permite recibir el mensaje


	// 8 bits de datos
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);

	// Baudrate 9600 parap 16MHz
	UBRR0 = 103;
}

// Enviar un carácter
void writeChar(char c)
{
	while (!(UCSR0A & (1<<UDRE0))); // Esperar buffer vacío
	UDR0 = c;
}

// Enviar string

void writeString(char *string)
{
	for (uint8_t i = 0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}
/****************************************/
// NON-Interrupt subroutines

	// Convertir número a ASCII y enviarlo
	void writeNumber(uint8_t num)
	{
		char buffer[4];
		uint8_t i = 0;

		if (num == 0)
		{
			writeChar('0');
			return;
		}

		while (num > 0)
		{
			buffer[i++] = (num % 10) + '0';
			num /= 10;
		}

		for (int8_t j = i - 1; j >= 0; j--)
		{
			writeChar(buffer[j]);
		}
	}

	/****************************************/
	// Mostrar valor ASCII en LEDs
	void displayASCII(uint8_t value)
	{
		// PD2–PD7 ? bits 0–5
		PORTD = (PORTD & 0x03) | ((value << 2) & 0b11111100);

		// PB0–PB1 ? bits 6–7
		PORTB = (PORTB & 0b11111100) | ((value >> 6) & 0b00000011);
	}

/****************************************/
// Interrupt routines
// ISR UART RX
/*
ISR(USART_RX_vect)
{
	uint8_t bufferRX = UDR0;

	// lo que veo:
	writeChar(bufferRX);

	// Mostrar ASCII
	writeString(" Respuesta: \r\n");
	writeString(" Valor en tabla ASCII: ");
	writeNumber(bufferRX);
	writeString("\r\n");
	// Mostrar en LEDs
	displayASCII(bufferRX);
	

	// Control LED PB5
	if (bufferRX == '1')
	{
		//PORTB |= (1<<PORTB5);
		
		writeString(" Opción -Leer pot\r\n");
		writeString("\r\n");
	}
	
	
	else if (bufferRX == '2')
	{
		//parte de lectura en ascii
		writeString("Modo ASCII\r\n");
		writeString("ASCII: ");
		writeNumber(bufferRX);
		writeString("\r\n");

		displayASCII(bufferRX);
	}
	else
	{
		writeString("Opción invalida\r\n");
	}
	// volver a mostrar menú
	printMenu();

}
*/

ISR(USART_RX_vect)
{
	uint8_t bufferRX = UDR0;

	writeChar(bufferRX);
	writeString("\r\n");
	

	// MODO 0 ? MENU
	if (modo == 0)
	{
		
		if (bufferRX == '1')
		{
			PORTB |= (1<<PORTB5); // encender LED
			writeString("Leyendo potenciometro...xd\r\n");
			// aquí iría la lectura como tal
			//********************************
			ADC_Read(2); // dummy
			uint16_t valor = ADC_Read(2);

			itoa(valor, buffer, 10);

			writeString(" -> ADC: ");
			writeString(buffer);
			writeString("\r\n");
			
			//**********************************
		}
		
		else if (bufferRX == '2')
		{
			PORTB |= (1<<PORTB5); // encender LED
			writeString("Ingresa un caracter para ver su ASCII:\r\n");
			modo = 2; // CAMBIAS DE MODO
			return;   // NOTA: se sale sin mostrar menú
		}
		else
		{
			writeString("Opcion invalida\r\n");
		}

		printMenu();
	}


//Lesctura en ASCII: 

	//  Modo 2---ESPERANDO CARACTER
	else if (modo == 2)
	{
		
		writeString("ASCII: ");
		writeNumber(bufferRX);
		writeString("\r\n");

		displayASCII(bufferRX);
	

		modo = 0; //regresar al menú
		printMenu();
	}
}



//Segun tengo entendido:  half-duplex.
//Pero podría ser full-duplex, si se busca que funcione todo al mismo tiempo. 
// del micro a la pantalla y de la pantalla al micro.