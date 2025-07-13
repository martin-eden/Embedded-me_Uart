// UART interrupts

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
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

// Enable interrupt when data frame received
void TReceivedDataInterrupt::On()
{
  Uart->EnableOnReceivedInterrupt = true;
}

// Disable interrupt when data frame received
void TReceivedDataInterrupt::Off()
{
  Uart->EnableOnReceivedInterrupt = false;
}

// Enable interrupt when no data to send
void TReadyToSendInterrupt::On()
{
  Uart->EnableOnReadyToSendInterrupt = true;
}

// Disable interrupt when no data to send
void TReadyToSendInterrupt::Off()
{
  Uart->EnableOnReadyToSendInterrupt = false;
}

// Enable interrupt when data frame sent
void TSentDataInterrupt::On()
{
  Uart->EnableOnSentInterrupt = true;
}

// Disable interrupt when data frame sent
void TSentDataInterrupt::Off()
{
  Uart->EnableOnSentInterrupt = false;
}

/*
  2024-11 # #
  2025-07-13
*/
