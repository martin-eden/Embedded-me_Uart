// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-11-06
*/

/*
  Ambassadort, savant and servant

  SetSpeed(Baud)

    Is ambassador. Makes sure UART is set to given speed.

  CalculateBitDuration_ut()

    Savant. Does math and returns result in hardware units.

  SetBitDuration_ut()

    Servant. Writes value at specific memory location.
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

// Set bit duration. Custom unit. Not all durations can be set
TBool Freetown::SetBitDuration_ut(
  TUint_2 BitDuration_ut
)
{
  union TCounterLimit
  {
    TUint_2 Value : 12;
    struct
    {
      TUint_1 Value_LowByte : 8;
      TUint_1 Value_HighByte : 4;
    };
  };

  TCounterLimit * CounterLimit = (TCounterLimit *) 196;

  // Memory value is 12-bit word

  /*
    We're setting limit value for (0, N) "for" loop.

    It will always run at least once.
  */

  if (BitDuration_ut == 0)
    return false;

  TUint_2 Limit = BitDuration_ut - 1;

  // Max value we can store
  TUint_2 MaxLimit = (1 << 12) - 1;

  if (Limit > MaxLimit)
    return false;

  /*
    Hardware magic occurs at writing low byte of counter.
    So we're writing high byte first.
  */

  CounterLimit->Value_HighByte = (Limit >> 8) & 0xFF;
  CounterLimit->Value_LowByte = Limit & 0xFF;

  return true;
}

/*
  2024-10-28
  2024-10-29
  2024-11-06 Moved here SetBitDuration_ut() too
*/
