/*
 * SCT.c
 *
 *  Created on: Feb 28, 2017
 *      Author: Joey
 */

#include "board.h"
#include "Arduino_Header.h"

/* Setup board specific pin muxing for SCT */
static void app_setup_pin(void)
{
	/* Enable SWM clock before altering SWM */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

#if (defined(BOARD_NXP_LPCXPRESSO_812) || defined(BOARD_LPC812MAX))
	/* Connect SCT output 0 to LED pin PIO7, SCT output 1 to PIO17 */
	Chip_SWM_MovablePinAssign(SWM_CTOUT_1_O, 6);
	Chip_SWM_MovablePinAssign(SWM_CTOUT_0_O, 7);
#elif defined(BOARD_NXP_LPCXPRESSO_824)
	/* Connect SCT output 0 to LED pin PIO7, SCT output 1 to PIO17 */
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT0_O, arduino10);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT1_O, arduino8);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT2_O, arduino7);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT3_O, arduino5);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT4_O, arduino4);
	Chip_SWM_MovablePinAssign(SWM_SCT_OUT5_O, arduino2);
#else
	/* Configure your own SCT pin muxing here if needed */
#warning "No SCT pin muxing defined"
#endif

	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}
