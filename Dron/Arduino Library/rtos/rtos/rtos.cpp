

#include "rtos.h"

rtos::rtos() {
};

unsigned long rtos_init(void) {

	cli();//stop interrupts
	
		  //set timer0 interrupt at 200Hz ali perioda= 5000 microsekund   !!!!!!pozor, imamo pogresek 8micro sekund,--> 4992 periodo!!!!
	TCCR0A = 0;// set entire TCCR2A register to 0
	TCCR0B = 0;// same for TCCR2B
	TCNT0 = 0;//initialize counter value to 0
			  // set compare match register for 2khz increments
	OCR0A = 77;// = (16*10^6) / (2000*1024) - 1 (must be <256)
				// turn on CTC mode
	TCCR0A |= (1 << WGM01);
	// Set CS01 and CS00 bits for 64 prescaler
	TCCR0B |= (1 << CS02) | (1 << CS00);
	// enable timer compare interrupt
	TIMSK0 |= (1 << OCIE0A);

	// to je start, bom dal loceno, vec nadzora
	// sei();//allow interrupts

	return 0;


}

void rtos_enable(void) {
	sei();
}

void rtos_disable(void) {
	cli();
}

extern rtos_task_t *rtos_task_list[];
extern uint32_t rtos_num_tasks;

// RTOS Scheduler
void SysTick_Handler(void) {

	static int task_state = TASK_COMPLETE;

	if (task_state != 0) {
		while (1);
	}
	task_state = TASK_RUNNING;//tuki bi moral 26bit pogledat kako je postavljen
	static unsigned long i = 0;

	rtos_task_list[i]->function();
	i++;

	if (i == 4) {
		i = 0;
	}

	task_state = TASK_COMPLETE;
}