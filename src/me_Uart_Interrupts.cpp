// UART interrupts

/*
  Author: Martin Eden
  Last mod.: 2024-11-19
*/

/*
   Receiver            Interrupts
  -------------       -------------
  (*) Has data  -[ ]-
  -------------       -------------

   Transmitter           Interrupts
  ---------------       -------------
  (*) Ready       -[ ]-
  (*) Frame sent  -[ ]-
  ---------------       -------------
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

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
  2024-11-08
  2024-11-09
*/
