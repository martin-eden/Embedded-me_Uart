// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2024-11-08
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
  Register->EnableTransmitter = true;
}

void TTransmitter::Off()
{
  Register->EnableTransmitter = false;
}

TBool TTransmitter::IsReady()
{
  return Register->ReadyToTransmit;
}

void TTransmitter::Put(TUint_1 Data)
{
  *Buffer = Data;
}

/*
  2024-11-08
*/
