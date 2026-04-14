/*
 * adc.h
 *
 * Created: 14/04/2026 12:33:27
 *  Author: grett
 */ 

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

// Inicializa el ADC
void ADC_Init(void);

// Lee un canal del ADC (0–7)
uint16_t ADC_Read(uint8_t channel);

#endif