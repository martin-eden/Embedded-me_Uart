// [me_Uart] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-10-26
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_UartSpeeds.h>
#include <me_Console.h>

void setup()
{
  me_Uart::Init(me_UartSpeeds::Arduino_Normal_Bps);

  Console.Print("[me_Uart] Start.");
  RunTest();
  Console.Print("[me_Uart] Done.");
}

void loop()
{
}

// --

void RunTest()
{
  Console.Print("If you can see this it means [me_Uart] works");
  Console.Print("for transmission.");
  Console.Print("");
  Console.Print("We'll print lot of lines to measure transfer time.");

  TUint_4 StartTime = millis();

  TUint_2 NumLines = 180;
  TUint_1 LineSize = 64; // 63 + 1 for newline

  for (TUint_2 Counter = 0; Counter < NumLines; ++Counter)
  {
    for (TUint_1 Byte = 64; Byte < 128; ++Byte)
      me_Uart::SendByte(Byte);

    me_Uart::SendByte(10);
  }

  TUint_4 StopTime = millis();

  Console.Write("Transferred");
  Console.Print((TUint_2) NumLines * LineSize);
  Console.Write("bytes in");
  Console.Print(StopTime - StartTime);
  Console.Write("ms.");
  Console.EndLine();
}

/*
  2024-10-25
  2024-10-26
*/
