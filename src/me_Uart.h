// UART access

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  // [Core] Set-up for given speed (no parity, 8 data bits, 1 stop bit)
  TBool Init(TUint_4 Speed);

  // [Core] Get byte (if we has one)
  TBool GetByte(TUint_1 * Value);

  // [Core] Send byte
  void SendByte(TUint_1 Value);

  /*
    [Convenience] Typical UART connection speeds

    Here are only values that we consider practical in 2025.
  */
  const TUint_4
    Speed_9k_Bps = 9600,
    Speed_57k_Bps = 57600,
    Speed_115k_Bps = 115200,
    Speed_250k_Bps = 250000,
    Speed_500k_Bps = 500000,
    Speed_1M_Bps = 1000000,
    Speed_2M_Bps = 2000000;

  // [Convenience] Wait byte. Maybe forever
  void WaitByte(TUint_1 * Value);
}

/*
  2024 # # # # # # # # # # # # #
  2025-08-25
  2025-08-29
*/
