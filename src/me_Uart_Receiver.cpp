// UART receiver

/*
  Author: Martin Eden
  Last mod.: 2024-11-14
*/

/*
                         Receiver
  -----------------------------------------------------------
       |               Data                |
   On  |                                   | (*) Has data
   [x] |   7   6   5   4   3   2   1   0   | (o) With errors
    |  |  (o) (o) (o) (o) (o) (o) (o) (o)  |    __________
   [ ] |                                   |   | Get next |
       |                                   |   `~~~~~~~~~~'
  -----------------------------------------------------------
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

void TReceiver::On()
{
  UartState->EnableReceiver = true;
}

void TReceiver::Off()
{
  UartState->EnableReceiver = false;
}

TBool TReceiver::HasData()
{
  return UartState->Received;
}

TBool TReceiver::AreErrors()
{
  return (UartState->FrameHasErrors != 0);
}

TUint_1 TReceiver::Get()
{
  return UartState->Buffer;
}

/*
  2024-11-08
*/
