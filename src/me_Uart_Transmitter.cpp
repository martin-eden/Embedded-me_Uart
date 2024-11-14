// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2024-11-14
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

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

void TTransmitter::On()
{
  UartState->EnableTransmitter = true;
}

void TTransmitter::Off()
{
  UartState->EnableTransmitter = false;
}

TBool TTransmitter::IsReady()
{
  return UartState->ReadyToSend;
}

void TTransmitter::Put(TUint_1 Data)
{
  UartState->Buffer = Data;
}

/*
  2024-11-08
*/
