// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
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

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

/*
  Calculate how many units can be fit in given length

  Helper.

  Used for calculating bit duration.
*/
TBool GetNumUnitsForLength(
  TUint_4 * NumUnits,
  TUint_4 Length,
  TUint_4 UnitSize
)
{
  if (UnitSize == 0)
    return false;

  // Rounding to nearest integer
  *NumUnits = ((2 * Length / UnitSize) + 1) / 2;

  return true;
}

/*
  Speed (bits per second)

  Internal structure.

  Speed is described as number of time units
  and unit size (normal or double-size).
*/
struct TSpeedSetting
{
  TUint_2 BitDuration_ut;
  TBool UseNormalSpeed;
};

/*
  Calculate bit duration in hardware time units

  We're inverting speed to delays.

  We're willing to use smaller time units for precision.

  If there are too much small time units, we'll use larger ones.
*/
TBool CalculateBitDuration_ut(
  TSpeedSetting * HwSpeed,
  TUint_4 Speed_Bps
)
{
  // Bit duration we can store is 12-bit value
  const TUint_4 MaxDuration = (1 << 12);

  TUint_4 CycleDuration;
  TUint_4 CyclesPerSecond;
  TBool UsedDoubleUnits;

  // Duration of cycle is 8 ticks
  CyclesPerSecond = F_CPU / 8;
  UsedDoubleUnits = false;

  // Calculate duration
  if (!GetNumUnitsForLength(&CycleDuration, CyclesPerSecond, Speed_Bps))
    return false;

  // Speed too slow:
  if (CycleDuration > MaxDuration)
  {
    // Duration of cycle is 16 ticks
    CyclesPerSecond = F_CPU / 16;
    UsedDoubleUnits = true;

    // Calculate duration
    if (!GetNumUnitsForLength(&CycleDuration, CyclesPerSecond, Speed_Bps))
      return false;

    // Speed still too slow. Fail
    if (CycleDuration > MaxDuration)
      return false;
  }

  // Speed too high. Fail
  if (CycleDuration == 0)
    return false;

  HwSpeed->BitDuration_ut = (TUint_2) CycleDuration;
  HwSpeed->UseNormalSpeed = UsedDoubleUnits;

  return true;
}

/*
  Set transceiver speed
*/
TBool TSpeedSetter::SetSpeed(
  TUint_4 Speed_Bps
)
{
  TSpeedSetting SpeedSetting;
  TBool IsOkay;

  IsOkay = CalculateBitDuration_ut(&SpeedSetting, Speed_Bps);

  if (!IsOkay)
    return false;

  // assert( 1 <= SpeedSetting.BitDuration_ut <= 4096 )
  SetBitDuration_ut(SpeedSetting.BitDuration_ut - 1);

  if (SpeedSetting.UseNormalSpeed)
    SetNormalSpeed();
  else
    SetDoubleSpeed();

  return true;
}

/*
  Estimate speed using CPU freq and bit duration
*/
TBool TSpeedSetter::GetSpeed(
  TUint_4 * Speed_Bps
)
{
  TUint_2 BitDuration;
  TUint_1 TicksPerCycle;

  BitDuration = Uart->BitDuration.Duration + 1;

  if (Uart->UseDoubleSpeed)
    TicksPerCycle = 8;
  else
    TicksPerCycle = 16;

  return GetNumUnitsForLength(Speed_Bps, F_CPU / TicksPerCycle, BitDuration);
}

/*
  Set bit duration

  Custom time unit. Not all durations can be set.

  We're setting "N" value for "for" loop from 0 to N.
  So it will always run at least once.
*/
void TSpeedSetter::SetBitDuration_ut(
  TUint_2 BitDuration
)
{
  /*
    Hardware magic occurs when writing low byte to that USART register

    So we're going into trouble of converting word to record.
    To be sure that low byte of that word is written last.
  */

  me_Uart_Bare::TBitDuration DurationRec;

  DurationRec.Duration = BitDuration;

  Uart->BitDuration.Duration_HighByte = DurationRec.Duration_HighByte;
  Uart->BitDuration.Duration_LowByte = DurationRec.Duration_LowByte;
}

/*
  Use normal transceiver speed
*/
void TSpeedSetter::SetNormalSpeed()
{
  Uart->UseDoubleSpeed = false;
}

/*
  Use double transceiver speed
*/
void TSpeedSetter::SetDoubleSpeed()
{
  Uart->UseDoubleSpeed = true;
}

/*
  2024-10 # #
  2024-11 # # #
  2024-12 # #
  2025-07-13
*/
