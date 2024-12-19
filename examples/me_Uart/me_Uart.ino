// [me_Uart] Bare-bones test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-12-19
*/

/*
  Loopback test for ATmega328/P at 16 MHz

  Echoes everything until it gets "^".

  This transceiver works up to 2 Mbps. (2 Mbps is maximum speed
  for 16 MHz.) But in real code limiting factor is your stream
  processing time.
*/

/*
  Footprint

    Memory   Size     Date
      9       542  2024-11-09
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>

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
  using namespace me_Uart;

  TUint_1 Byte;

  Init(Speed_2M_Bps);

  SendByte('>');

  while (true)
  {
    WaitByte(&Byte);

    if (Byte == '^')
      break;

    SendByte(Byte);
  }

  SendByte('<');
}

/*
  2024-10 # # # #
  2024-12-19
*/
