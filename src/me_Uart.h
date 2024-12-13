// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-12-18
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

  // ( Wrappers as TOperation
  TBool Op_GetByte(TAddress Data, TAddress Unused);
  TBool Op_PutByte(TAddress Data, TAddress Unused);
  // )
}

/*
  2024-10 ####
  2024-11 ##
  2024-12-18
*/
