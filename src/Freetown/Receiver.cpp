// UART receiver

/*
  Author: Martin Eden
  Last mod.: 2026-11-21
*/

/*
                         Receiver
  -----------------------------------------------------------------
       |               Data                |
   On  |                                   | ,~~~~~~~~~~.
   [x] |   7   6   5   4   3   2   1   0   | | Get next | (o) Good
    |  |  (o) (o) (o) (o) (o) (o) (o) (o)  | `~~~~~~~~~~'
   [ ] |                                   |
       |                                   |
  -----------------------------------------------------------------
*/

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

// ( Receiver switch
TBool TReceiverSwitch::IsOn()
{
  return Uart->EnableReceiver;
}

void TReceiverSwitch::On()
{
  Uart->EnableReceiver = true;
}

void TReceiverSwitch::Off()
{
  Uart->EnableReceiver = false;
}
// )

/*
  Receive byte

  There is hardware magic at accessing transceiver buffer.
  Reading or writing it updates flags. So flags must be read first.

  def
  (
    get
    (
      ( assert ( Received 1 ) )
      ( let ( errors FrameHasErrors ) )
      ( write ( Buffer ) )
      ( assert ( errors 0 ) )
    )
  )
*/
TBool TReceiver::Get(
  TUint_1 * Data
)
{
  if (!Uart->Received)
    return false;

  TBool DataHasErorrs = (Uart->FrameHasErrors != 0);

  *Data = Uart->Buffer;

  if (DataHasErorrs)
    return false;

  return true;
}

/*
  2024 #
  2025 # # #
  2026-11-21
*/
