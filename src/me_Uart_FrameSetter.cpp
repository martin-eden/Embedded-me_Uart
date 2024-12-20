// UART frame setter

/*
  Author: Martin Eden
  Last mod.: 2024-12-19
*/

/*
                         Frame setter

       Bits       |         Parity       | Double stop
  --------------  | -------------------- | ------------
                  |   On    |  Invert    |     On
      |           |   [ ]   |   [ ]      |     [ ]
    9-8-7 6 5     |    |    |    |       |      |
    | ^ | | |     |  -[x]-  |   [x]      |    -[x]-
      |           |    |    |            |      |
  --------------  | -------------------- |-------------

*/

#include <me_Uart.h>

#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

// Set frame size to 8 bits
void TFrameSetter::Set8BitsFrame()
{
  // Value 011
  Uart->FrameSize_3 = 0;
  Uart->FrameSize_12 = 3;
}

// Set no parity
void TFrameSetter::SetNoParity()
{
  Uart->Parity = 0;
}

// Set one stop bit in a frame
void TFrameSetter::SetOneStopBit()
{
  Uart->StopBits = 0;
}

/*
  2024-11 #
*/
