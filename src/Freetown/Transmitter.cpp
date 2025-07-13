// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
*/

/*
                         Transmitter
  -----------------------------------------------------------
       |                 Data                   |
   On  |     7   6   5   4   3   2   1   0      | (o) Ready
   [x] |    [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]     |  ______
    |  |     |   |   |   |   |   |   |   |      | | Send |
   [ ] |    [x] [x] [x] [x] [x] [x] [x] [x]     | `~~~~~~'
       |                                        |
  -----------------------------------------------------------
*/

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

void TTransmitter::On()
{
  Uart->EnableTransmitter = true;
}

void TTransmitter::Off()
{
  Uart->EnableTransmitter = false;
}

TBool TTransmitter::IsReady()
{
  return Uart->ReadyToSend;
}

void TTransmitter::Put(TUint_1 Data)
{
  Uart->Buffer = Data;
}

/*
  2024-11 #
  2024-12 #
  2025-07-13
*/
