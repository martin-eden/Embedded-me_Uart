// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-12-13
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

/*
  Ambassador, savant and servant

  SetSpeed(Baud)

    Is ambassador. Makes sure UART is set to given speed.

  CalculateBitDuration_ut()

    Savant. Does math and returns result in hardware units.

  SetBitDuration_ut()

    Servant. Writes value at specific memory location.
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

// Set transceiver speed
TBool TSpeedSetter::SetSpeed(
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

    if (SetBitDuration_ut(BitDuration_ut))
    {
      SetDoubleSpeed();
      return true;
    }
  }

  // Use large time units
  {
    UseSmallUnits = false;
    BitDuration_ut =
      CalculateBitDuration_ut(Speed_Bps, UseSmallUnits);

    if (SetBitDuration_ut(BitDuration_ut))
    {
      SetNormalSpeed();
      return true;
    }
  }

  // Nope. Probably speed is too low to be expressed in our delay units
  return false;
}

// Calculate bit duration in hardware time units
TUint_4 TSpeedSetter::CalculateBitDuration_ut(
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
TBool TSpeedSetter::SetBitDuration_ut(
  TUint_2 BitDuration_ut
)
{
  const TUint_2 MaxLimit = (1 << 12) - 1;

  if (BitDuration_ut == 0)
    return false;

  TUint_2 Limit = BitDuration_ut - 1;

  if (Limit > MaxLimit)
    return false;

  TBitDuration Arg;
  Arg.Value = Limit;

  // Hardware magic occurs at writing low byte of counter.
  Uart->BitDuration.Value_HighByte = Arg.Value_HighByte;
  Uart->BitDuration.Value_LowByte = Arg.Value_LowByte;

  return true;
}

// Use normal transceiver speed
void TSpeedSetter::SetNormalSpeed()
{
  Uart->UseDoubleSpeed = false;
}

// Use double transceiver speed
void TSpeedSetter::SetDoubleSpeed()
{
  Uart->UseDoubleSpeed = true;
}

/*
  2024-10-28
  2024-10-29
  2024-11-06
  2024-11-07
  2024-11-08
*/
