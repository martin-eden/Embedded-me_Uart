// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-11-08
*/

/*
  Here is unsophisticated design. Three core functions.

  We are not using external buffers for transmission and receiving.
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  // Set-up for given speed (no parity, 8 data bits, 1 stop bit)
  TBool Init(TUint_4 Speed);

  // Send byte
  void SendByte(TUint_1 Value);

  // Receive byte
  TBool ReceiveByte(TUint_1 * Value);

  // Await byte
  void AwaitByte(TUint_1 * Value);
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
  2024-11-08
*/
