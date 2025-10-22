// [me_Uart] Bare-bones test/demo

/*
  Author: Martin Eden
  Last mod.: 2025-10-22
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

    Memory  Size     Date         Changes note
      9      542  2024-11-09
      9      544  2025-08-26  Changed speed to 115k
      9      562  2025-09-23  Internal Switch class
      9      940  2025-10-22  Using standalone speed calculation
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>

/*
  Echo test
*/
void EchoTest()
{
  TUint_1 Byte;

  me_Uart::SendByte('>');

  while (true)
  {
    me_Uart::WaitByte(&Byte);

    if (Byte == '^')
      break;

    me_Uart::SendByte(Byte);
  }

  me_Uart::SendByte('<');
}

void setup()
{
  me_Uart::Init(me_Uart::Speed_115k_Bps);

  EchoTest();
}

void loop()
{
}

/*
  2024 # # # # #
  2025-08-26
*/
