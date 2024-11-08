// UART access. Frontend functions

/*
  Author: Martin Eden
  Last mod.: 2024-11-05
*/

/*
  No Freetown punks here!

  Here are only white-collar functions with situation awareness.
*/

#include "me_Uart.h"

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart;

// Set-up for given speed (no parity, 8 data bits, 1 stop bit)
TBool me_Uart::Init(
  TUint_4 Speed_Bps
)
{
  if (!Freetown::SetSpeed(Speed_Bps))
    return false;

  Freetown::SetAsyncMode();
  Freetown::SetOneStopBit();
  Freetown::SetNoParity();
  Freetown::Set8BitsFrame();

  Freetown::DisableOnReceiveCompleteInterrupt();
  Freetown::DisableOnTransmitCompleteInterrupt();
  Freetown::DisableOnEmptyBufferInterrupt();

  Freetown::EnableReceiver();
  Freetown::EnableTransmitter();

  return true;
}

// Send byte
void me_Uart::SendByte(
  TUint_1 Value
)
{
  // This "while" shouldn't take long
  while (!Freetown::ReadyToTransmit());

  Freetown::Buffer_PutByte(Value);
}

// Receive byte
TBool me_Uart::ReceiveByte(
  TUint_1 * Value
)
{
  /*
    There is hardware magic at accessing transceiver buffer.
    Reading or writing it updates flags. So flags must be read first.
  */

  if (!Freetown::ReceivedByte())
    return false;

  // Received with errors. Will read but discard
  while (Freetown::FrameHasErrors())
    Freetown::Buffer_ExtractByte(Value);

  Freetown::Buffer_ExtractByte(Value);

  return true;
}

// Await byte. Maybe forever
void me_Uart::AwaitByte(
  TUint_1 * Value
)
{
  while (!ReceiveByte(Value));
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-11-05
*/
