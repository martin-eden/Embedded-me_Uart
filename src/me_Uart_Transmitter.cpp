// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2024-12-19
*/

/*
                         Transmitter
  -----------------------------------------------------------
       |                 Data                   |
   On  |     7   6   5   4   3   2   1   0      | (*) Ready
   [x] |    [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]     |  ______
    |  |     |   |   |   |   |   |   |   |      | | Send |
   [ ] |    [x] [x] [x] [x] [x] [x] [x] [x]     | `~~~~~~'
       |                                        |
  -----------------------------------------------------------
*/

#include <me_Uart.h>

#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

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
*/
