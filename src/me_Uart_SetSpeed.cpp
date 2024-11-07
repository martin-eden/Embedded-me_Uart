// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-11-07
*/

/*
  Ambassador, savant and servant

  SetSpeed(Baud)

    Is ambassador. Makes sure UART is set to given speed.

  CalculateBitDuration_ut()

    Savant. Does math and returns result in hardware units.

  SetBitDuration_ut()

    Servant. Writes value at specific memory location.
*/

/*
                           Speed setter

     |
     | *
     |
  1M |  *                                                [ ]
     |                                                    |  Speed x 2
     |   *                                               [x]
     |
     |
     |      *
     |
     |          *
     |              *
     |                    *
     |                             *
     |                                           *
     ---|-----^----------------------------------|----
        0     |                                4095
           Duration
*/

#include "me_Uart.h"

#include <me_BaseTypes.h>

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

struct TDoubleSpeedSwitch
{
  TUint_1 Unused_1 : 1;
  TBool UseDoubleSpeed : 1;
  TUint_1 Unused_2 : 6;
};

TDoubleSpeedSwitch * DoubleSpeedSwitch = (TDoubleSpeedSwitch *) 192;

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

  // Nope. Probably speed is too low to be expressed in our delay units
  return false;
}

// Calculate bit duration in hardware time units
TUint_4 Freetown::CalculateBitDuration_ut(
  TUint_4 Speed_Bps,
  TBool UseDoubleSpeed
)
{
  TUint_1 TicksPerCycle;

  if (UseDoubleSpeed)
    TicksPerCycle = 8;
  else
    TicksPerCycle = 16;

  // Those "/ 2" and " + 1" are needed for rounding.

  return
    ((F_CPU / (TicksPerCycle / 2)) / Speed_Bps + 1) / 2;
}

/*
  Set bit duration

  Custom time unit. Not all durations can be set.

  We're setting limit value for (0, N) "for" loop.
  So it will always run at least once.
*/
TBool Freetown::SetBitDuration_ut(
  TUint_2 BitDuration_ut
)
{
  const TUint_2 MaxLimit = (1 << 12) - 1;

  if (BitDuration_ut == 0)
    return false;

  TUint_2 Limit = BitDuration_ut - 1;

  if (Limit > MaxLimit)
    return false;

  TCounterLimit CounterLimit_FromArg;
  CounterLimit_FromArg.Value = Limit;

  // Hardware magic occurs at writing low byte of counter.
  CounterLimit->Value_HighByte = CounterLimit_FromArg.Value_HighByte;
  CounterLimit->Value_LowByte = CounterLimit_FromArg.Value_LowByte;

  return true;
}

// Use normal transceiver speed
void Freetown::SetNormalSpeed()
{
  // Value 0. Register 1, offset 1

  DoubleSpeedSwitch->UseDoubleSpeed = false;
}

// Use double transceiver speed
void Freetown::SetDoubleSpeed()
{
  // Value 1. Register 1, offset 1

  DoubleSpeedSwitch->UseDoubleSpeed = true;
}

/*
  2024-10-28
  2024-10-29
  2024-11-06 Moved here SetBitDuration_ut() too
  2024-11-07 and SetNormalSpeed(), SetDoubleSpeed() too
*/
