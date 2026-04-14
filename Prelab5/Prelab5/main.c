/*
 * Prelab5.c
 *
 * Created: 14/04/26
 * Author: Grettel Escobedo
 * Description: 
	 Utilizando un canal analógico realice la conversión ADC y configure el módulo
	 PWM 1 del microcontrolador para poder generar una señal de PWM que controle
	 todo el rango de movimiento de un servo. Esto lo debe hacer creando una librería
	 propia para el PWM.

 */
/****************************************/
// Encabezado (Libraries)
#include "PWM1.h"
#include "adc.h"
/****************************************/
// Function prototypes
/****************************************/
// Main Function

int main(void)
{
	PWM1_Init();
	ADC_Init();

	uint16_t adc_value;
	uint16_t duty;

	while (1)
	{
		adc_value = ADC_Read(7); // lee mi  pot en A7

		// Mapear ADC ? servo
		//duty = 2000 + ((adc_value * 2000) / 1023); //Funciona, pero poquito 
		// se combierte el tiempo en pulso. 
		
		//duty = 1500 + ((adc_value * 2500) / 1023); No funcionó
		duty = 1000 + ((adc_value * 4000) / 1023);

		PWM1_SetDuty(duty); //se mueve el servo con el valor de duty
	}
}


/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines