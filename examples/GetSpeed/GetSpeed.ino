// Get actual UART connection speed

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
*/

/*
  At compile time we know assumed CPU frequency, F_CPU.
  Bit duration on microcontroller is calculated using that
  and required speed.

  Here we are calling reverse calculation: get speed from
  knowing bit duration and CPU frequency.
*/

#include <me_BaseTypes.h>
#include <me_Console.h>

#include <me_Uart.h>

#include "Freetown/Interface.h" // low-level module with fancy stuff

TBool GetRealSpeed(TUint_4 * Speed_Bps)
{
  using
    me_Uart_Freetown::TSpeedSetter;

  TSpeedSetter SpeedSetter;

  return SpeedSetter.GetSpeed(Speed_Bps);
}

void setup()
{
  // Works from 300 to 2000000 speeds. Amazing
  if (!me_Uart::Init(115200))
    return;

  TUint_4 RealSpeed;

  if (!GetRealSpeed(&RealSpeed))
  {
    Console.Print("Failed to get real speed.");
    return;
  }

  Console.Write("Real speed is (bps)");
  Console.Print(RealSpeed);
  Console.EndLine();
}

void loop()
{
}

/*
  2024-12-13
  2025-07-13
*/
