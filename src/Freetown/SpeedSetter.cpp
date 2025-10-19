// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2025-10-19
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
  Calculate bit duration in hardware time units
*/
static TBool CalculateBitDuration_ut(
  me_HardwareClockScaling::TClockScale * ClockScale,
  TUint_4 Speed_Bps
)
{
  return
    me_HardwareClockScaling::CalculateClockScale(
      ClockScale,
      Speed_Bps,
      me_HardwareClockScaling::AtMega328::GetSpec_Uart()
    );
}

/*
  Set transceiver speed
*/
TBool TSpeedSetter::SetSpeed(
  TUint_4 Speed_Bps
)
{
  me_HardwareClockScaling::TClockScale ClockScale;

  if (!CalculateBitDuration_ut(&ClockScale, Speed_Bps))
    return false;

  SetBitDuration_ut(ClockScale.CounterLimit);

  if (ClockScale.Prescale_PowOfTwo == 3)
    SetDoubleSpeed();
  else if (ClockScale.Prescale_PowOfTwo == 4)
    SetNormalSpeed();
  else
    return false;

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
  2025-10-19
*/
