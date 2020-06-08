/*
 * config.h
 *
 * Created: 07.06.2020 13:21:57
 *  Author: FHoch
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC_MAX					1023
#define TIMER_TICK				(double) 5e-6
#define STATUS_LED_PIN			(1 << 5)
#define STATUS_LED_PORT			PORTB
#define STATUS_LED_DDR			DDRB

#define STEP_PORT				PORTD
#define STEP_PIN				(1 << 5)
#define STEP_DDR				DDRD

#define DIR_PORT				PORTD
#define DIR_PIN					(1 << 6)
#define DIR_DDR					DDRD

#define EN_PORT					PORTD
#define EN_PIN					(1 << 7)
#define EN_DDR					DDRD

#define SW_F_PORT				PINB
#define SW_F_PIN				(1 << 0)
#define SW_F_DDR				DDRB

#define SW_R_PORT				PINB
#define SW_R_PIN				(1 << 1)
#define SW_R_DDR				DDRB

// Fokus Travel Setup
#define MOTOR_TEETH				20
#define PULLEY_TEETH			80
#define TRAVEL_PR				10				// 10mm Travel per full rotation
#define STEP_SIZE				((double) 1.80)	// Degress per Step
#define MICROSTEPPING			16				// mircosteps per step
#define STEPS_PR				((double) 360.0 / (STEP_SIZE / MICROSTEPPING))

#define MAX_SPEED				15.0				// mm/s
#define MIN_SPEED				0.05					// mm/s
#define OFF_DELAY				3000



void init();
double speedToPeriod(double speed);
uint16_t getADCValue();
double adcToSpeed(uint16_t adcVal);
void enableDriver();
void disableDriver();


#endif /* CONFIG_H_ */