#ifndef _rtos_h_
#define _rtos_h_


#define TASK_COMPLETE 0
#define TASK_RUNNING 1

class rtos {

public:
	rtos();
	/* podatkovni tip; kazalec na funk */
	typedef void(*ptr_func)(void);

	struct _task {
		//to je sedaj smatran kot nek tip spremenljivke
		ptr_func function;
		unsigned long last_tick;
		unsigned long priority;
		unsigned long interval;

	};

	typedef struct _task rtos_task_t;


	/*RTOS init
	*
	*slice_us: time slice in microseconds
	*	returns: 		0 on success
	*					1 if fail
	*/

	unsigned long rtos_init(void);//sam moras poracunati in dolocit frekvenco, ker sicer bo delna napaka zaokrozevanja...

	//	Enable SysTick timer
	void rtos_enable(void);

	//disable SysTick timer
	void rtos_disable(void);

	void SysTick_Handler(void);



	//private:


};

#endif