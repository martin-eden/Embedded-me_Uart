// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-11-09
*/

/*
  Here is unsophisticated design. Three core functions.
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  // Set-up for given speed (no parity, 8 data bits, 1 stop bit)
  TBool Init(TUint_4 Speed);

  // Send byte
  void SendByte(TUint_1 Value);

  // Get byte (if we has one)
  TBool GetByte(TUint_1 * Value);

  // Await byte. Maybe forever
  void WaitByte(TUint_1 * Value);
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
  2024-11-08
*/
