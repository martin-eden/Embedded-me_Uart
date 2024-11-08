// UART access. Backend functions

/*
  Author: Martin Eden
  Last mod.: 2024-11-06
*/

/*
  Greasers, a lot of them

  They don't call each other. No awareness. They care only about
  correctly reading/writing designated memory location.

  I could write "sbi" or direct assignments. I could embed them into
  code. I chose not to.

  Names are better than comments to uncomprehensible code.
  Comments are just poor man's names.
*/

#include <me_Uart_Freetown.h>

#include <me_BaseTypes.h>

using namespace me_Uart::Freetown;

TRegister * Register = (TRegister *) 192;
TCounterLimit * CounterLimit = (TCounterLimit *) 196;
TUint_1 * UartBuffer = (TUint_1 *) 198;

// ( Freetown

/*
  Set bit duration

  Custom time unit. Not all durations can be set.

  We're setting limit value for (0, N) "for" loop.
  So it will always run at least once.
*/
TBool me_Uart::Freetown::SetBitDuration_ut(
  TUint_2 BitDuration_ut
)
{
  const TUint_2 MaxLimit = (1 << 12) - 1;

  if (BitDuration_ut == 0)
    return false;

  TUint_2 Limit = BitDuration_ut - 1;

  if (Limit > MaxLimit)
    return false;

  TCounterLimit CounterLimit_FromArg;
  CounterLimit_FromArg.Value = Limit;

  // Hardware magic occurs at writing low byte of counter.
  CounterLimit->Value_HighByte = CounterLimit_FromArg.Value_HighByte;
  CounterLimit->Value_LowByte = CounterLimit_FromArg.Value_LowByte;

  return true;
}

// Use normal transceiver speed
void me_Uart::Freetown::SetNormalSpeed()
{
  // Value 0. Register 1, offset 1

  Register->UseDoubleSpeed = false;
}

// Use double transceiver speed
void me_Uart::Freetown::SetDoubleSpeed()
{
  // Value 1. Register 1, offset 1

  Register->UseDoubleSpeed = true;
}

// Set asynchronous UART mode
void me_Uart::Freetown::SetAsyncMode()
{
  // Value 00. Register 3, offset 6

  Register->TransceiverMode = 0;

  ClearPolarityBit();
}

/*
  Clear polarity bit

  Polarity is synchronous mode stuff.

  For async mode this bit should be set to zero.
*/
void me_Uart::Freetown::ClearPolarityBit()
{
  // Value 0. Register 3, offset 0

  Register->Polarity = 0;
}

// Set one stop bit in a frame
void me_Uart::Freetown::SetOneStopBit()
{
  // Value 0. Register 3, offset 3

  Register->StopBits = 0;
}

// Set no parity
void me_Uart::Freetown::SetNoParity()
{
  // Value 00. Register 3, offset 4

  Register->Parity = 0;
}

// Set frame size to 8 bits
void me_Uart::Freetown::Set8BitsFrame()
{
  /*
    Value 011

    Highest bit lives in register 2, offset 2.
    Other two are in register 3 offset 1.
  */

  Register->FrameSize_12 = 3;
  Register->FrameSize_3 = 0;
}

// Disable interrupt when data frame received
void me_Uart::Freetown::DisableOnReceiveCompleteInterrupt()
{
  // Value 0. Register 2, offset 7

  Register->EnableOnReceiveCompleteInterrupt = false;
}

// Disable interrupt when data frame sent
void me_Uart::Freetown::DisableOnTransmitCompleteInterrupt()
{
  // Value 0. Register 2, offset 6

  Register->EnableOnTransmitCompleteInterrupt = false;
}

// Disable interrupt when no data
void me_Uart::Freetown::DisableOnEmptyBufferInterrupt()
{
  // Value 0. Register 2, offset 5

  Register->EnableOnEmptyBufferInterrupt = false;
}

// Disable receiver
void me_Uart::Freetown::DisableReceiver()
{
  // Value 0. Register 2, offset 4

  Register->EnableReceiver = false;
}

// Enable receiver
void me_Uart::Freetown::EnableReceiver()
{
  // Value 1. Register 2, offset 4

  Register->EnableReceiver = true;
}

// Disable transmitter
void me_Uart::Freetown::DisableTransmitter()
{
  // Value 0. Register 2, offset 3

  Register->EnableTransmitter = false;
}

// Enable transmitter
void me_Uart::Freetown::EnableTransmitter()
{
  // Value 1. Register 2, offset 3

  Register->EnableTransmitter = true;
}

// Return true when transmitter is idle
TBool me_Uart::Freetown::ReadyToTransmit()
{
  // Register 1, offset 5. Read-only

  return Register->ReadyToTransmit;
}

// Return true when there is data in receive buffer
TBool me_Uart::Freetown::ReceivedByte()
{
  // Register 1, offset 7. Read-only

  return Register->ReceivedByte;
}

// Return true if frame is received with errors
TBool me_Uart::Freetown::FrameHasErrors()
{
  // Value should be not 000. Register 1, offset 2

  return (Register->FrameHasErrors != 0);
}

// Put byte to transceiver buffer
void me_Uart::Freetown::Buffer_PutByte(
  TUint_1 Data
)
{
  /*
    Writing to this memory location updates flags and
    initiates hardware transmitter.
  */

  *UartBuffer = Data;
}

// Extract byte from transceiver buffer
void me_Uart::Freetown::Buffer_ExtractByte(
  TUint_1 * Data
)
{
  /*
    Reading this memory location updates flags and
    initiates hardware receiver.
  */

  *Data = *UartBuffer;
}

// ) Freetown

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
  2024-11-07
*/
