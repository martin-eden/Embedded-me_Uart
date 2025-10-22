// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2025-10-21
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
  [Internal] Set bit duration
*/
static void SetBitDuration_ut(
  TUint_2 BitDuration
)
{
  /*
    Hardware magic occurs when writing low byte to that USART register

    So we're went into trouble of converting word to record.
    To be sure that low byte of that word is written last.
  */

  me_Uart_Bare::TBitDuration DurationRec;

  DurationRec.Duration = BitDuration;

  Uart->BitDuration.Duration_HighByte = DurationRec.Duration_HighByte;
  Uart->BitDuration.Duration_LowByte = DurationRec.Duration_LowByte;
}

/*
  [Internal] Use normal transceiver speed
*/
static void SetNormalSpeed()
{
  Uart->UseDoubleSpeed = false;
}

/*
  [Internal] Use double transceiver speed
*/
static void SetDoubleSpeed()
{
  Uart->UseDoubleSpeed = true;
}

/*
  [Internal] Set clock scale
*/
static TBool SetClockScale(
  me_HardwareClockScaling::TClockScale ClockScale
)
{
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
  [Internal] Get clock scale
*/
static me_HardwareClockScaling::TClockScale GetClockScale()
{
  me_HardwareClockScaling::TClockScale ClockScale;

  ClockScale.CounterLimit = Uart->BitDuration.Duration;

  if (Uart->UseDoubleSpeed)
    ClockScale.Prescale_PowOfTwo = 3;
  else
    ClockScale.Prescale_PowOfTwo = 4;

  return ClockScale;
}

/*
  Set transceiver speed
*/
TBool TSpeedSetter::SetSpeed(
  TUint_4 Speed_Bps
)
{
  me_HardwareClockScaling::TClockScale ClockScale;

  if (
    !me_HardwareClockScaling::CalculateClockScale_Specs(
      &ClockScale,
      Speed_Bps,
      me_HardwareClockScaling::AtMega328::GetSpec_Uart()
    )
  )
    return false;

  if (!SetClockScale(ClockScale))
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
  return
    me_HardwareClockScaling::CalculateFrequency(Speed_Bps, GetClockScale());
}

/*
  2024 # # # # # # #
  2025-07-13
  2025-10-19
*/
