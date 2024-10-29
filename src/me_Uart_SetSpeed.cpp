// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-10-29
*/

/*
  Savant and ambassador

  SetSpeed(Baud)

    Is ambassador. Makes sure UART is set to given speed.

  CalculateBitDuration_ut()

    Savant. Does math and returns result in hardware units.
*/

#include "me_Uart.h"

#include <me_BaseTypes.h>

using namespace me_Uart;

// Set transceiver speed
TBool Freetown::SetSpeed(
  TUint_4 Speed_Bps
)
{
  /*
    We're inverting speed to delays.

    We're willing to use smaller time units for precision.

    If there are too much small time units, we'll use larger ones.
  */

  TBool UseSmallUnits;
  TUint_2 BitDuration_ut;

  // Use small time units
  {
    UseSmallUnits = true;
    BitDuration_ut =
      CalculateBitDuration_ut(Speed_Bps, UseSmallUnits);

    if (Freetown::SetBitDuration_ut(BitDuration_ut))
    {
      Freetown::SetDoubleSpeed();
      return true;
    }
  }

  // Use large time units
  {
    UseSmallUnits = false;
    BitDuration_ut =
      CalculateBitDuration_ut(Speed_Bps, UseSmallUnits);

    if (Freetown::SetBitDuration_ut(BitDuration_ut))
    {
      Freetown::SetNormalSpeed();
      return true;
    }
  }

  // Nope. Probably speed is too slow to be expressed in our delay units
  return false;
}

// Calculate bit duration in hardware time units
TUint_4 Freetown::CalculateBitDuration_ut(
  TUint_4 Speed_Bps,
  TBool UseDoubleSpeed
)
{
  TUint_1 TicksInCycle;

  if (UseDoubleSpeed)
    TicksInCycle = 8;
  else
    TicksInCycle = 16;

  /*
    Those "/ 2" and " + 1" are needed for rounding.
  */

  return
    ((F_CPU / (TicksInCycle / 2)) / Speed_Bps + 1) / 2;
}

/*
  2024-10-28
  2024-10-29
*/
