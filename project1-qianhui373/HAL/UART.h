/*
 * UART.h
 *
 *  Created on: Dec 31, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Ever since the new API update in mid 2019, the UART configuration struct's
// name changed from [eUSCI_UART_Config] to [eUSCI_UART_ConfigV1]. If your code
// does not compile, uncomment the alternate #define and comment the original
// one.
// -----------------------------------------------------------------------------
// #define UART_Config eUSCI_UART_Config
#define UART_Config eUSCI_UART_ConfigV1

// An enum outlining what baud rates the UART_construct() function can use in
// its initialization.
enum _UART_Baudrate
{
    BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, NUM_BAUD_CHOICES
};
typedef enum _UART_Baudrate UART_Baudrate;

// TODO: Write an overview explanation of what this UART struct does, and how it
//       interacts with the functions below. Consult <HAL/Button.h> and
//       <HAL/LED.h> for examples on how to do this.

// comment answer: in this struct, it has the four data which can be measured and transmitted.
struct _UART
{
    UART_Config config;

    uint32_t moduleInstance;
    uint32_t port;
    uint32_t pins;
};
typedef struct _UART UART;

// Initializes a UART connection.
UART UART_construct(UART_Baudrate baudChoice);

// TODO: Write a comment which explains what each of these functions does. In the
//       header, prefer explaining WHAT the function does, as opposed to HOW it is
//       implemented.

/* comment answer:
 in the following part code, it gives four functions that will be implement in UART.c.
UART_getChar is to get the needed information.
 UART_hasChar is used to scan the data and verify it get the correct information.
UART_canSend is used to verify whether the data can be transmitted to MobaXterm.
UART_putChar is used to transmit the data to MobaXterm and put it into it.*/

char UART_getChar(UART* uart_p);
bool UART_hasChar(UART* uart_p);
bool UART_canSend(UART* uart_p);
void UART_putChar(UART* uart_p, char c);

// Updates the UART baudrate to use the new baud choice.
void UART_updateBaud(UART* uart_p, UART_Baudrate baudChoice);

#endif /* HAL_UART_H_ */
