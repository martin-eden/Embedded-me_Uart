// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-10-29
*/

/*
  Savant, medium and ambassador

  SetSpeed(Baud)

    Is ambassador. Makes sure UART is set to given speed

  CalculateBitDuration_us()

    Savant. Doing math in SI.

  FromMicros_us()

    Medium. Converting SI to hardware units given circumstances.
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

  TUint_4 BitDuration_us =
    CalculateBitDuration_us(Speed_Bps);

  TBool UseSmallUnits;
  TUint_2 BitDuration_ut;

  // Use small time units
  {
    UseSmallUnits = true;
    BitDuration_ut =
      FromMicros_ut(BitDuration_us, UseSmallUnits);

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
      FromMicros_ut(BitDuration_us, UseSmallUnits);

    if (Freetown::SetBitDuration_ut(BitDuration_ut))
    {
      Freetown::SetNormalSpeed();
      return true;
    }
  }

  // Nope. Probably speed is too slow to be expressed in our delay units
  return false;
}

// Calculate bit duration in microseconds
TUint_4 Freetown::CalculateBitDuration_us(
  TUint_4 Speed_Bps
)
{
  /*
    Explaining formulae

      1 second = 1000000 microseconds

      115200 bits are transferred in that duration

      What is duration of one bit?

      1000000 / 115200 ~= 8.68 ~= 8

      8 * 115200 = 921600 // 8% error
      9 * 115200 = 1036800 // 3 % error

      So we'll do rounding by adding half of 115200 to 1000000:

      1057600 / 115200 ~= 9.18 ~= 9
  */

  return
    (1000000 + Speed_Bps / 2) / Speed_Bps;
}

// Convert microseconds to "number of delays" unit
TUint_2 Freetown::FromMicros_ut(
  TUint_4 BitDuration_us,
  TBool UseDoubleSpeed
)
{
  /*
    Things are more hairy at hardware level.

    There is idle cycle (for i = 0, N)(wait (8 or 16) ticks).
    It's needed to keep line stable for that time.

    We're converting duration in microseconds to that counter
    value.

    (N + 1) * TicksInCycle * TickDuration_us == BitDuration_us
    N == (BitDuration_us / (TicksInCycle * TickDuration_us)) - 1

    TickDuration_us = 1 / CpuTicksPerSecond * 1000000 // = 0.0625 us for 16 MHz

    0.065 is too bad for integer calculations.

    So we're wrapping all in one expression:

      N + 1 = BitDuration_us / (TicksInCycle * (1 / F_CPU) * 1000000)
        = BitDuration_us / (TicksInCycle * 1000000 / F_CPU)
        = BitDuration_us * F_CPU / (TicksInCycle * 1000000)
  */

  TUint_1 TicksInCycle;

  if (UseDoubleSpeed)
    TicksInCycle = 8;
  else
    TicksInCycle = 16;

  return
    BitDuration_us * F_CPU / (TicksInCycle * 1000000) - 1;
}

/*
  2024-10-28
  2024-10-29
*/
