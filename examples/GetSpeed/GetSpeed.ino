// Get actual UART connection speed

/*
  Author: Martin Eden
  Last mod.: 2024-12-13
*/

/*
  At compile time we know assumed CPU frequency, F_CPU.
  Bit duration on microcontroller is calculated using that
  and required speed.

  Here we are calling reverse calculation to get speed.
*/

#include <me_BaseTypes.h>
#include <me_Console.h>

#include <me_Uart.h>

#include <me_Uart_Freetown.h> // low-level module with fancy stuff

TUint_4 GetRealSpeed()
{
  using
    me_Uart::Freetown::TSpeedSetter;

  TSpeedSetter SpeedSetter;

  return SpeedSetter.GetSpeed();
}

void setup()
{
  // Works from 300 to 2000000 speeds. Amazing
  me_Uart::Init(115200);

  TUint_4 RealSpeed = GetRealSpeed();
  Console.Write("Real speed is (bps)");
  Console.Print(RealSpeed);
  Console.EndLine();
}

void loop()
{
}

/*
  2024-12-13
*/
