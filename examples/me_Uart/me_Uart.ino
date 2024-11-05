// [me_Uart] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-10-30
*/

/*
  For ATmega328/P at 16 MHz

    * This transceiver works up to 1 Mbps (10^6 bits per second)

    * It works on up to 2 Mbps for transmission.
      (2 Mbps is maximum possible speed for 16 MHz.)

  But in real code limiting factor is your stream processing time.
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_UartSpeeds.h>
#include <me_Console.h>

void setup()
{
  RunTest();
}

void loop()
{
}

// --

void RunTest()
{
  TUint_4 Speed_Bps = me_UartSpeeds::Bps_1M;

  me_Uart::Init(Speed_Bps);

  Console.Print("[me_Uart] Start.");

  Console.Write("Speed is");
  Console.Print(Speed_Bps);
  Console.Write("baud.");
  Console.EndLine();

  Console.Print("");
  Console.Print("We'll print a lot of lines to measure transfer time.");

  TUint_4 StartTime = millis();

  TUint_2 NumLines = 16;
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

  Console.Print("Echo until '~' character..");

  TUint_1 Byte;
  do
  {
    me_Uart::AwaitByte(&Byte);
    me_Uart::SendByte(Byte);
  } while (Byte != '~');

  Console.Print("[me_Uart] Done.");
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-30
*/
