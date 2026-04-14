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
	PWM2_Init();
	ADC_Init();

	   uint16_t adc_servo1;
	   uint16_t adc_servo2;
	   //uint16_t adc_led;

	   uint16_t duty_servo1;
	   uint16_t duty_servo2;
	  // uint8_t duty_led;

	while (1)
	{
		adc_servo1 = ADC_Read(7); // lee mi  pot en A7
		adc_servo2 = ADC_Read(2); // PC2 nuevo adc NUEVO*
		 //adc_led    = ADC_Read(3); // PC3, esta será la parte donde esta la lectura del pot 3


		// Mapear ADC ? servo
		//intentos:
				//duty = 2500 + ((adc_value * 1000UL) / 1023); //Funciona 
		
				/*
				duty = 2000 + ((adc_value * 2000UL) / 1023); Funciona para 90
				if (duty < 2200) duty = 2200;
				if (duty > 3800) duty = 3800;
				*/
				//duty = 1800 + ((adc_value * 2400UL) / 1023); //Funciona pero se muve 140 aprox
				
		//funciona: 
		duty_servo1= 1500 + ((adc_servo1 * 3000UL) / 1023);
		duty_servo2 = 1000 + ((adc_servo2 * 4000) / 1023)

		//PWM1_SetDuty(duty); //se mueve el servo con el valor de duty
		PWM1_SetDuty(duty_servo1);
		PWM2_SetDuty(duty_servo2);

	}
}


/****************************************/
// NON-Interrupt subroutines
/****************************************/
// Interrupt routines