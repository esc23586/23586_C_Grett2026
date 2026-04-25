/*
 * adc.h
 *
 * Created: 23/04/2026 12:23:38
 *  Author: grett
  Descripción del archivo:
 Archivo donde se le indica al compilador, existen las funciones, especialmente para mi adc
 */

 #ifndef ADC_H_
 #define ADC_H_

 #include <stdint.h>

 // Inicializa el ADC
 void ADC_Init(void);

 // Lee un canal del ADC (0 a7) y devuelve el valor
 uint16_t ADC_Read(uint8_t channel);

 #endif /* ADC_H_ */