// UART interrupts

/*
  Author: Martin Eden
  Last mod.: 2025-09-22
*/

/*
   Receiver            Interrupts
  -------------       -------------
  (o) Has data  -[ ]-
  -------------       -------------

   Transmitter           Interrupts
  ---------------       -------------
  (o) Ready       -[ ]-
  (o) Frame sent  -[ ]-
  ---------------       -------------
*/

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

// ( Interrupt when received data
void TReceivedDataInterrupt::Get(
  TBool * State
)
{
  *State = Uart->EnableOnReceivedInterrupt;
}

void TReceivedDataInterrupt::Set(
  TBool State
)
{
  Uart->EnableOnReceivedInterrupt = State;
}
// )

// ( Interrupt when no data to send
void TReadyToSendInterrupt::Get(
  TBool * State
)
{
  *State = Uart->EnableOnReadyToSendInterrupt;
}

void TReadyToSendInterrupt::Set(
  TBool State
)
{
  Uart->EnableOnReadyToSendInterrupt = State;
}
// )

// ( Interrupt when data frame sent
void TSentDataInterrupt::Get(
  TBool * State
)
{
  *State = Uart->EnableOnSentInterrupt;
}

void TSentDataInterrupt::Set(
  TBool State
)
{
  Uart->EnableOnSentInterrupt = State;
}
// )

/*
  2024-11 # #
  2025-07-13
  2025-09-22 Using Switch interface
*/
