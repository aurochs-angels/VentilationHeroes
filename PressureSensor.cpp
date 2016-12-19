/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "I2C.h"
#include "ITM_write.h"
#include <cstring>
// TODO: insert other include files here
#define TICKRATE_HZ (1000)	/* 100 ticks per second */
static volatile uint32_t ticks;
static volatile uint32_t counter;

// TODO: insert other definitions and declarations here
extern "C" {
void SysTick_Handler(void) {

	if (counter > 0) {
		counter--;
	}
}
}
void Sleep(uint32_t time) {
	counter = time;
	while (counter > 0) {
		__WFI();
	}
}

int main(void) {

#if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();
	// Set the LED to the state of "On"
	Board_LED_Set(0, true);
#endif
#endif

	// TODO: insert code here
	SysTick_Config(Chip_Clock_GetSysTickClockRate() / TICKRATE_HZ);
	ITM_init();
	I2C i2c(0, 100000);

	// Force the counter to be placed into memory
	volatile static int i = 0;
	// Enter an infinite loop, just incrementing a counter
	while (1) {
		i++;
		i2c.pressureRead();
		Sleep(1000);
	}
	return 0;
}
