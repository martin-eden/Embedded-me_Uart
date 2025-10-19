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

#include <me_HardwareClockScaling.h>

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
*/
TBool CalculateBitDuration_ut(
  TSpeedSetting * HwSpeed,
  TUint_4 Speed_Bps
)
{
  me_HardwareClockScaling::TClockScale ClockScale;
  TBool IsOk;

  IsOk =
    me_HardwareClockScaling::CalculateClockScale(
      &ClockScale,
      Speed_Bps,
      me_HardwareClockScaling::AtMega328::GetSpec_Uart()
    );

  if (!IsOk)
    return false;

  HwSpeed->UseNormalSpeed = (ClockScale.Prescale_PowOfTwo == 4);
  HwSpeed->BitDuration_ut = ClockScale.CounterLimit + 1;

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

  if (!CalculateBitDuration_ut(&SpeedSetting, Speed_Bps))
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
  me_HardwareClockScaling::TClockScale ClockScale;

  ClockScale.CounterLimit = Uart->BitDuration.Duration;

  if (Uart->UseDoubleSpeed)
    ClockScale.Prescale_PowOfTwo = 3;
  else
    ClockScale.Prescale_PowOfTwo = 4;

  return me_HardwareClockScaling::CalculateFrequency(Speed_Bps, ClockScale);
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
  2024 # # # # # # #
  2025-07-13
  2025-10-18
*/
