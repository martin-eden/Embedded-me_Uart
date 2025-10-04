// UART frame setter

/*
  Author: Martin Eden
  Last mod.: 2025-10-04
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

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

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
  Uart->UseTwoStopBits = false;
}

/*
  2024-11 #
  2025-07-13
*/
