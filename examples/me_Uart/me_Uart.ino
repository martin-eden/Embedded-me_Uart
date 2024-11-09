// [me_Uart] Bare-bones test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-11-08
*/

/*
  For ATmega328/P at 16 MHz

    * This transceiver works up to 2 Mbps
      (2 Mbps is maximum possible speed for 16 MHz.)

  But in real code limiting factor is your stream processing time.
*/

/*
  Footprint

    Memory   Size     Date
      9       542  2024-11-09
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
  me_Uart::Init(me_UartSpeeds::Bps_2M);

  me_Uart::SendByte('>');

  TUint_1 Byte;
  while (true)
  {
    me_Uart::WaitByte(&Byte);
    if (Byte == '^')
      break;
    me_Uart::SendByte(Byte);
  }

  me_Uart::SendByte('<');
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-30
*/
