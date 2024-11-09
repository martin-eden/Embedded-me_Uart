// UART access. Backend functions

/*
  Author: Martin Eden
  Last mod.: 2024-11-06
*/

/*
  Greasers, a lot of them

  They don't call each other. No awareness. They care only about
  correctly reading/writing designated memory location.

  I could write "sbi" or direct assignments. I could embed them into
  code. I chose not to.

  Names are better than comments to uncomprehensible code.
  Comments are just poor man's names.
*/

#include <me_Uart_Freetown.h>

#include <me_BaseTypes.h>

using namespace me_Uart::Freetown;

TRegister * Register = (TRegister *) 192;
TCounterLimit * CounterLimit = (TCounterLimit *) 196;
TUint_1 * UartBuffer = (TUint_1 *) 198;

// ( Freetown

// Disable interrupt when data frame received
void me_Uart::Freetown::DisableOnReceiveCompleteInterrupt()
{
  // Value 0. Register 2, offset 7

  Register->EnableOnReceiveCompleteInterrupt = false;
}

// Disable interrupt when data frame sent
void me_Uart::Freetown::DisableOnTransmitCompleteInterrupt()
{
  // Value 0. Register 2, offset 6

  Register->EnableOnTransmitCompleteInterrupt = false;
}

// Disable interrupt when no data
void me_Uart::Freetown::DisableOnEmptyBufferInterrupt()
{
  // Value 0. Register 2, offset 5

  Register->EnableOnEmptyBufferInterrupt = false;
}

// ) Freetown

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
  2024-11-07
*/
