// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2026-11-21
*/

/*
                         Transmitter
  ---------------------------------------------------------
       |                 Data                   |
   On  |     7   6   5   4   3   2   1   0      | ,~~~~~~.
   [x] |    [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]     | | Send |
    |  |     |   |   |   |   |   |   |   |      | `~~~~~~'
   [ ] |    [x] [x] [x] [x] [x] [x] [x] [x]     |
       |                                        |
  ---------------------------------------------------------
*/

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

// ( Transmitter switch
TBool TTransmitterSwitch::IsOn()
{
  return Uart->EnableTransmitter;
}

void TTransmitterSwitch::On()
{
  Uart->EnableTransmitter = true;
}

void TTransmitterSwitch::Off()
{
  Uart->EnableTransmitter = false;
}
// )

/*
  Send byte

  def
  (
    put
    (
      ( wait ( assert ( ReadyToSend 1 ) ) )
      ( set ( Buffer read ) )
      ( wait ( assert ( Sent 1 ) ) )
      ( set ( Sent 1 ) )
    )
  )
*/
void TTransmitter::Send(
  TUint_1 Data
)
{
  while (!Uart->ReadyToSend);
  Uart->Buffer = Data;
  while (!Uart->Sent);
  Uart->Sent = true;
}

/*
  2024 # #
  2025 # # #
  2026-11-21
*/
