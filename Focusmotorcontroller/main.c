/*
 * Focusmotorcontroller.c
 *
 * Created: 05.06.2020 12:50:49
 * Author : FHoch
 */ 

#include "config.h"

volatile uint16_t timerCounter = 0;
volatile uint32_t stepCounter = 0;
volatile uint8_t flag10ms = 0;
volatile uint32_t periodZ = 0;

int8_t prevState = 0;

void init() {
	OCR1A = 1;
	TIMSK1 |=  (1 << OCIE1A);
	TCCR1B |=  (1 << WGM12) | (1 << CS11);	
	sei();
	
	STATUS_LED_DDR |= STATUS_LED_PIN;
	STEP_DDR  |= STEP_PIN;
	DIR_DDR  |= DIR_PIN;
	EN_DDR  |= EN_PIN;
	
	SW_F_DDR &= ~SW_F_PIN;
	SW_R_DDR &= ~SW_R_PIN;	
	
	// Select Vref=AVcc
	ADMUX |= (1<<REFS0);
	//set prescaller to 128 and enable ADC
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADEN);
}

int main(void)
{
	init();
    disableDriver();
	while (1) {
		if (flag10ms) {
			flag10ms = 0;
			double period = speedToPeriod(adcToSpeed(getADCValue()));
			periodZ = (uint32_t) (period / TIMER_TICK);
			uint8_t state = 0;
			if (SW_F_PORT & SW_F_PIN) {
				state = 1;
				DIR_PORT |= DIR_PIN;
			} else if (SW_R_PORT & SW_R_PIN) {
				state = -1;
				DIR_PORT &= ~DIR_PIN;
			} 
			
			
			if (state != prevState) {
				if (state) {
					enableDriver();
					stepCounter = periodZ;
				} else disableDriver();
				
			}
			prevState = state;
			
		}
		
    }
}

ISR (TIMER1_COMPA_vect) { // every 2us
	stepCounter--;
	if (stepCounter == 0) {
		STEP_PORT |= STEP_PIN;
		stepCounter = periodZ;
	} else {
		STEP_PORT &= ~STEP_PIN;
	}
	
	timerCounter++;
	if (timerCounter == 5000) {
		flag10ms = 1;
		timerCounter = 0;
	}
	STATUS_LED_PORT ^= STATUS_LED_PIN;
}


double speedToPeriod(double speed) {
	double RPS = speed / TRAVEL_PR; // revolutions per second
	double motorRPS = (RPS * PULLEY_TEETH) / MOTOR_TEETH;
	double stepFrequency = motorRPS * STEPS_PR;
	return 1/stepFrequency;
}

uint16_t getADCValue() {
	ADMUX &= ~((1 << MUX0) | (1 << MUX1) |(1 << MUX2) | (1 << MUX3)); // RESET REGISTER
	ADCSRA |= (1 << ADSC);
	
	while(ADCSRA & (1 << ADSC));
	
	uint8_t low = ADCL;
	uint8_t high = ADCH;
	return (high << 8) | low;
}

void enableDriver() {
	DIR_PORT &= DIR_PIN;
}
void disableDriver() {
	DIR_PORT |= DIR_PIN;
}

double adcToSpeed(uint16_t adcVal) {
	if (adcVal > ADC_MAX) adcVal = ADC_MAX;
	
	return MIN_SPEED + MAX_SPEED * (double) adcVal / ADC_MAX;
} 
