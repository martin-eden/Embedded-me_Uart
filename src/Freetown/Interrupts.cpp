// UART interrupts

/*
  Author: Martin Eden
  Last mod.: 2025-09-23
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
TBool TReceivedDataInterrupt::IsOn()
{
  return Uart->EnableOnReceivedInterrupt;
}

void TReceivedDataInterrupt::On()
{
  Uart->EnableOnReceivedInterrupt = true;
}

void TReceivedDataInterrupt::Off()
{
  Uart->EnableOnReceivedInterrupt = false;
}
// )

// ( Interrupt when no data to send
TBool TReadyToSendInterrupt::IsOn()
{
  return Uart->EnableOnReadyToSendInterrupt;
}

void TReadyToSendInterrupt::On()
{
  Uart->EnableOnReadyToSendInterrupt = true;
}

void TReadyToSendInterrupt::Off()
{
  Uart->EnableOnReadyToSendInterrupt = false;
}
// )

// ( Interrupt when data frame sent
TBool TSentDataInterrupt::IsOn()
{
  return Uart->EnableOnSentInterrupt;
}

void TSentDataInterrupt::On()
{
  Uart->EnableOnSentInterrupt = true;
}

void TSentDataInterrupt::Off()
{
  Uart->EnableOnSentInterrupt = false;
}
// )

/*
  2024-11 # #
  2025-07-13
  2025-09-22 Using Switch interface
  2025-09-23
*/
