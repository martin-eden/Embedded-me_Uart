// UART receiver

/*
  Author: Martin Eden
  Last mod.: 2025-09-22
*/

/*
                         Receiver
  -----------------------------------------------------------
       |               Data                |
   On  |                                   | (o) Has data
   [x] |   7   6   5   4   3   2   1   0   | (o) With errors
    |  |  (o) (o) (o) (o) (o) (o) (o) (o)  |    __________
   [ ] |                                   |   | Get next |
       |                                   |   `~~~~~~~~~~'
  -----------------------------------------------------------
*/

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

// ( Receiver switch
void TReceiverSwitch::Get(
  TBool * State
)
{
  *State = Uart->EnableReceiver;
}

void TReceiverSwitch::Set(
  TBool State
)
{
  Uart->EnableReceiver = State;
}
// )

TBool TReceiver::HasData()
{
  return Uart->Received;
}

TBool TReceiver::AreErrors()
{
  return (Uart->FrameHasErrors != 0);
}

TUint_1 TReceiver::Get()
{
  return Uart->Buffer;
}

/*
  2024-11 #
  2025-07-13
  2025-09-22
*/
