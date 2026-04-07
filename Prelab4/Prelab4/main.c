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
Contador:

*/

/****************************************/
// Encabezado (Libraries)
#include <avr/io.h>
#include <avr/interrupt.h>
/****************************************/
// Function prototypes
/****************************************/
// Main Function
int main (void)
{
	cli(); //deshabilitar interrupciones
	while(1)
	{
		
	}
}
/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines
