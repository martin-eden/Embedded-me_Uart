// Get actual UART connection speed

/*
  Author: Martin Eden
  Last mod.: 2025-08-26
*/

/*
  At compile time we know assumed CPU frequency, F_CPU.
  Bit duration on microcontroller is calculated using that
  and required speed.

  Here we are calling reverse calculation: get speed from
  knowing bit duration and CPU frequency.
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <Freetown/Interface.h> // for Speed Setter access

/*
  Get real transmission speed

  Real speed is determined by UART hardware delay counters.
*/
void GetRealSpeedTest()
{
  me_Uart_Freetown::TSpeedSetter SpeedSetter;
  TUint_4 RealSpeed;

  if (!SpeedSetter.GetSpeed(&RealSpeed))
  {
    Console.Print("Failed to get real speed.");
    return;
  }

  Console.Write("Real speed is (bps)");
  Console.Print(RealSpeed);
  Console.EndLine();
}

void setup()
{
  Console.Init();

  // Works from 300 to 2000000 speeds. Amazing
  if (!me_Uart::Init(115200))
    return;

  Console.Print("[GetSpeed] Calculate real transmission speed");

  GetRealSpeedTest();

  Console.Print("Done.");
}

void loop()
{
}

/*
  2024 #
  2025-07-13
  2025-08-26
*/
