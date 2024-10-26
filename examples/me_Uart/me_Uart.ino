// [me_Uart] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-10-26
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_UartSpeeds.h>

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
  me_Uart::TUartChannel Uart;

  Uart.Init(me_UartSpeeds::Arduino_Normal_Bps);

  while (true)
  {
    for (TUint_1 Byte = 64; Byte < 128; ++Byte)
    {
      Uart.SendByte(Byte);
      delay(100);
    }

    Uart.SendByte(10);

    delay(400);
  }
}

/*
  2024-10-25
  2024-10-26
*/
