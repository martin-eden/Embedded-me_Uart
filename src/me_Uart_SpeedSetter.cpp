// UART access. Speed calculation and setting

/*
  Author: Martin Eden
  Last mod.: 2024-12-15
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

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

// Quite generic function to do core calculation
TBool EstimateLength(
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

  Speed as described as number of time units
  and unit size.
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
  if (!EstimateLength(&CycleDuration, CyclesPerSecond, Speed_Bps))
    return false;

  // Speed too slow:
  if (CycleDuration > MaxDuration)
  {
    // Duration of cycle is 16 ticks
    CyclesPerSecond = F_CPU / 16;
    UsedDoubleUnits = true;

    // Calculate duration
    if (!EstimateLength(&CycleDuration, CyclesPerSecond, Speed_Bps))
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

// Set transceiver speed
TBool TSpeedSetter::SetSpeed(
  TUint_4 Speed_Bps
)
{
  TSpeedSetting SpeedSetting;
  TBool IsOkay;
  TBitDuration BitDuration;

  IsOkay = CalculateBitDuration_ut(&SpeedSetting, Speed_Bps);

  if (!IsOkay)
    return false;

  // assert( 1 <= SpeedSetting.BitDuration_ut <= 4096 )
  BitDuration.Value = SpeedSetting.BitDuration_ut - 1;

  SetBitDuration_ut(BitDuration);

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

  BitDuration = Uart->BitDuration.Value + 1;

  if (Uart->UseDoubleSpeed)
    TicksPerCycle = 8;
  else
    TicksPerCycle = 16;

  return EstimateLength(Speed_Bps, F_CPU / TicksPerCycle, BitDuration);
}

/*
  Set bit duration

  Custom time unit. Not all durations can be set.

  We're setting limit value for (0, N) "for" loop.
  So it will always run at least once.
*/
void TSpeedSetter::SetBitDuration_ut(
  TBitDuration BitDuration
)
{
  // Hardware magic occurs when writing low byte
  Uart->BitDuration.Value_HighByte = BitDuration.Value_HighByte;
  Uart->BitDuration.Value_LowByte = BitDuration.Value_LowByte;
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
  2024-10 ##
  2024-11 ###
  2024-12-13
  2024-12-15
*/
