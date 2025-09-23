// UART transmitter

/*
  Author: Martin Eden
  Last mod.: 2025-09-22
*/

/*
                         Transmitter
  ---------------------------------------------------------------
       |                 Data                   |
   On  |     7   6   5   4   3   2   1   0      | (o) Ready
   [x] |    [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]     |  ______
    |  |     |   |   |   |   |   |   |   |      | | Send |
   [ ] |    [x] [x] [x] [x] [x] [x] [x] [x]     | `~~~~~~'
       |                                        | (o) Sent
       |                                        |  ____________
       |                                        | | Clear Sent |
       |                                        | `~~~~~~~~~~~~'
  ---------------------------------------------------------------
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

TBool TTransmitter::IsReady()
{
  return Uart->ReadyToSend;
}

TBool TTransmitter::IsSent()
{
  return Uart->Sent;
}

void TTransmitter::ClearSentFlag()
{
  Uart->Sent = true;
}

void TTransmitter::Send(
  TUint_1 Data
)
{
  Uart->Buffer = Data;
}

/*
  2024 # #
  2025-07-13
  2025-09-05
  2025-09-22
*/
