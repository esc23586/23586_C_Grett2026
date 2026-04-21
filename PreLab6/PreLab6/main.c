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

/****************************************/
// Function prototypes
void intUART(void);
void writeChar(char c);
void writeString(char *string);

/****************************************/
// Main Function

/*
//Esta fue la primera versión pero con delay
void UART_init(unsigned int baud) {
	unsigned int ubrr = F_CPU/16/baud - 1;
	
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;
	
	UCSR0B = (1 << TXEN0); // habilitar transmisión
	
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits de datos
}

void UART_sendChar(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // esperar buffer vacío
	UDR0 = data;
}
*/

//Baud rates, 9600 bps y loop infinito de G
/*
int main(void) {
	UART_init(9600); // baudrate
	
	while (1) {
		UART_sendChar('G');
		_delay_ms(1000);
	}
}
*/

int main(void)
{
	cli();

	// LED en PB5 como salida
	DDRB |= (1<<DDB5);
	PORTB &= ~(1<<PORTB5); // inicialmente apagado

	intUART();// Inicializar UART

	sei();// (no es necesario si no usas RX, pero son buenas costumbres xd)

	//writeChar('G');  // solo una vez
	
	//si hubiera necesitado un string: 
	//writeString("Escribe 'a' para encender LED, 'b' para apagar\r\n");// Mensaje inicial

	
	// Si se necesita un enter:
	 writeString("G\r\n");
	
	while (1)
	{
		// no hacer nada
	}
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

// Enviar string (para futuro jsjijiji)
void writeString(char *string)
{
	for (uint8_t i = 0; string[i] != '\0'; i++)
	{
		writeChar(string[i]);
	}
}
/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines

// Interrupción al recibir dato
ISR(USART_RX_vect)
{
	uint8_t bufferRX = UDR0;

	writeChar(bufferRX); // eco (muestra lo que escribes)

	if (bufferRX == 'a')
	{
		PORTB |= (1<<PORTB5); // encender LED
		writeString(" LED ON\r\n");
	}
	else if (bufferRX == 'b')
	{
		PORTB &= ~(1<<PORTB5); // apagar LED
		writeString(" LED OFF\r\n");
	}
}


//Segun tengo entendido:  half-duplex.
//Pero podría ser full-duplex, si se busca que funcione todo al mismo tiempo. 
// del micro a la pantalla y de la pantalla al micro.