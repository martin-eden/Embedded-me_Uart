// [me_Uart] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-10-25
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_UartSpeeds.h>
#include <me_Console.h>

void setup()
{
  Serial.begin(me_UartSpeeds::Arduino_Normal_Bps);
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
  me_Uart::TUartChannel Uart;

  Uart.Init(me_UartSpeeds::Arduino_Normal_Bps);

  while (true)
  {
    for (TUint_1 Byte = 64; Byte < 128; ++Byte)
    {
      Uart.SendByte(Byte);
      delay(10);
    }

    Uart.SendByte(10);

    delay(100);
  }
}

/*
  2024-10-25
*/
