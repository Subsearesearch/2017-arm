/*
===============================================================================
 Name        : ROV_Controller_Prototype.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "board.h"
#include "chip.h"
#include "Arduino_Header.h"
#include "SCT.c"
#include "string.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define redT		0
#define orangeT		1
#define whiteT		2
#define greenT		3
#define lightBlueT	4
#define darkBlueT	5

#define FRT	redT
#define FLT	orangeT
#define BRT	whiteT
#define BLT	greenT
#define FVT	lightBlueT
#define BVT	darkBlueT

#define SCT_PWM_RATE   50		/* PWM frequency 50Hz */

/* Systick timer tick rate, to change duty cycle */
//#define TICKRATE_HZ     10000		/* 10 ms Tick rate */

//Board LED.
#define REDLED		0
#define GREENLED	1
#define BLUELED		2

#define SCT_PWM		LPC_SCT

//Joystick value indexes.
#define B0	0
#define B1	1
#define B2	2
#define B3	3

#define LX	0
#define LY	1
#define RX	2
#define RY	3

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

int8_t stick[] = {0, 0, 0, 0};
uint8_t button[] = {0, 0, 0, 0};
int32_t thruster[6];
uint8_t byteIn;
uint32_t byteIndex;
int bytes;

char start[] = "Hello to Raspberry Pi from LPC824.\r\n\r\nReady to run.\r\n\r\n";

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Ring buffer size */
#define UART_RB_SIZE 64

/* Set the default UART, IRQ number, and IRQ handler name */
#define LPC_USART       LPC_USART0
#define LPC_IRQNUM      UART0_IRQn
#define LPC_UARTHNDLR   UART0_IRQHandler

/* Default baudrate for testing */
#define UART_TEST_DEFAULT_BAUDRATE 115200

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RB_SIZE], txbuff[UART_RB_SIZE];

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* UART Pin mux function - note that SystemInit() may already setup your
   pin muxing at system startup */
static void Init_UART_PinMux(void)
{
	/* Enable the clock to the Switch Matrix */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_SWM);

#if (defined(BOARD_NXP_LPCXPRESSO_812) || defined(BOARD_LPC812MAX))

	Chip_Clock_SetUARTClockDiv(1);	/* divided by 1 */

	/* Connect the U0_TXD_O and U0_RXD_I signals to port pins(P0.4, P0.0) */
	Chip_SWM_DisableFixedPin(SWM_FIXED_ACMP_I1);
	Chip_SWM_MovablePinAssign(SWM_U0_TXD_O, 4);
	Chip_SWM_MovablePinAssign(SWM_U0_RXD_I, 0);

#elif defined(BOARD_NXP_LPCXPRESSO_824)
	/* We don't need DEBUG_UART so disable it */
	Chip_SWM_MovablePinAssign(SWM_U0_TXD_O, arduino1);
	Chip_SWM_MovablePinAssign(SWM_U0_RXD_I, arduino0);
#else
	/* Configure your own UART pin muxing here if needed */
#warning "No UART pin muxing defined"
#endif
	/* Disable the clock to the Switch Matrix to save power */
	Chip_Clock_DisablePeriphClock(SYSCTL_CLOCK_SWM);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */
void LPC_UARTHNDLR(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

//
//static void sendUART(const char *sendStr) {
//
//	while (*sendStr != '\0') {
//		Chip_UART_SendBlocking(LPC_USART0, sendStr, 1);
//		sendStr++;
//	}
//}

void HardFault_Handler(void) {

	Board_LED_Set(REDLED, true);

	while(1) {

	}
}

//void IntDefaultHandler(void) {
//
//	Board_LED_Set(BLUELED, true);
//
//	while(1) {
//
//	}
//}

/* Example entry point */
int main(void) {

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	Board_LED_Set(REDLED, false);

	/* Initialize the SCT as PWM and set frequency */
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, SCT_PWM_RATE);

	/* Setup Board specific output pin for SCT. */
	app_setup_pin();

	//Initialize the UART.
	Init_UART_PinMux();

	/* Setup UART */
	Chip_UART_Init(LPC_USART);
	Chip_UART_ConfigData(LPC_USART, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1);
	Chip_Clock_SetUSARTNBaseClockRate((115200 * 16), true);
	Chip_UART_SetBaud(LPC_USART, UART_TEST_DEFAULT_BAUDRATE);
	Chip_UART_Enable(LPC_USART);
	Chip_UART_TXEnable(LPC_USART);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_RB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, UART_INTEN_RXRDY);
	Chip_UART_IntDisable(LPC_USART, UART_INTEN_TXRDY);	/* May not be needed */

	/* preemption = 1, sub-priority = 1 */
	NVIC_EnableIRQ(LPC_IRQNUM);

	//Enable UART interrupt.
	Chip_UART_IntEnable(LPC_USART, UART_INTEN_RXRDY);
	NVIC_EnableIRQ(LPC_IRQNUM);

	/* Use SCT0_OUT1 pin */
	Chip_SCTPWM_SetOutPin(SCT_PWM, FRT + 1, FRT);
	Chip_SCTPWM_SetOutPin(SCT_PWM, FLT + 1, FLT);
	Chip_SCTPWM_SetOutPin(SCT_PWM, BRT + 1, BRT);
	Chip_SCTPWM_SetOutPin(SCT_PWM, BLT + 1, BLT);
	Chip_SCTPWM_SetOutPin(SCT_PWM, FVT + 1, FVT);
	Chip_SCTPWM_SetOutPin(SCT_PWM, BVT + 1, BVT);

	/* Start with 1500us pulse width */
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, FRT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, FLT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, BRT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, BLT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, FVT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_SetDutyCycle(SCT_PWM, BVT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * 1500) / 20000));
	Chip_SCTPWM_Start(SCT_PWM);

//	sendUART("Hello to Raspberry Pi from LPC824.\r\n\r\nReady to run.\r\n\r\n");

	//Transmit start message.
	Chip_UART_SendBlocking(LPC_USART, start, sizeof(start) - 1);
	//Send it again.
	Chip_UART_SendRB(LPC_USART, &txring, start, sizeof(start) - 1);

	while ((button[B0] == 0) && (button[B1] == 0) && (button[B2] == 0) && (button[B3] == 0)) {

		bytes = Chip_UART_ReadRB(LPC_USART, &rxring, &byteIn, 1);

		if (bytes > 0) {
			//Read button and joystick states.
			if (byteIndex < 4) {
				button[byteIndex] = byteIn;
			} else {
				stick[byteIndex - 4] = (int8_t) byteIn;
			}

			byteIndex++;

			if (byteIndex >= 8) {
				byteIndex = 0;
			}
		}
	}

	while (1) {

		bytes = Chip_UART_ReadRB(LPC_USART, &rxring, &byteIn, 1);

		if (bytes > 0) {
			//Read button and joystick states.
			if (byteIndex < 4) {
				button[byteIndex] = byteIn;
			} else {
				stick[byteIndex - 4] = (int8_t) byteIn;
			}

			byteIndex++;

			if (byteIndex >= 8) {
				byteIndex = 0;
			}
		}

		//convert data.
		thruster[FRT] = (int32_t) 1500 + (stick[RY] - stick[RX] - stick[LX]) * 200 / (256 * 3);
		thruster[FLT] = (int32_t) 1500 + (stick[RY] + stick[RX] + stick[LX]) * 200 / (256 * 3);
		thruster[BRT] = (int32_t) 1500 + (stick[RY] + stick[RX] - stick[LX]) * 200 / (256 * 3);
		thruster[BLT] = (int32_t) 1500 + (stick[RY] - stick[RX] + stick[LX]) * 200 / (256 * 3);
		thruster[FVT] = (int32_t) 1500 + stick[LY] * 200 / 256;
		thruster[BVT] = (int32_t) 1500 + stick[LY] * 200 / 256;

		//Update ESCs.
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, FRT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[FRT]) / 20000));
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, FLT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[FLT]) / 20000));
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, BRT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[BRT]) / 20000));
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, BLT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[BLT]) / 20000));
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, FVT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[FVT]) / 20000));
		Chip_SCTPWM_SetDutyCycle(SCT_PWM, BVT + 1, ((Chip_SCTPWM_GetTicksPerCycle(SCT_PWM) * thruster[BVT]) / 20000));

		//Wait for interrupt.
		__WFI();
	}
}
