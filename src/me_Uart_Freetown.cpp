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

#include "me_Uart.h"

#include <me_BaseTypes.h>

using namespace me_Uart;

// ( Freetown

struct TRegisters
{
  TBool Unused_1_1 : 1;
  TBool Unused_1_2 : 1;
  TUint_1 FrameHasErrors : 3;
  TBool ReadyToTransmit : 1;
  TBool Unused_1_7 : 1;
  TBool ReceivedByte : 1;

  TBool Unused_2_1 : 1;
  TBool Unused_2_2 : 1;
  TUint_1 FrameSize_3 : 1;
  TBool EnableTransmitter : 1;
  TBool EnableReceiver : 1;
  TBool EnableOnEmptyBufferInterrupt : 1;
  TBool EnableOnTransmitCompleteInterrupt : 1;
  TBool EnableOnReceiveCompleteInterrupt : 1;

  TUint_1 Polarity : 1;
  TUint_1 FrameSize_12 : 2;
  TUint_1 StopBits : 1;
  TUint_1 Parity : 2;
  TUint_1 TransceiverMode : 2;
};

volatile TRegisters * Registers = (TRegisters * ) 192;

TUint_1 * UartBuffer = (TUint_1 *) 198;

// Set asynchronous UART mode
void Freetown::SetAsyncMode()
{
  // Value 00. Register 3, offset 6

  Registers->TransceiverMode = 0;

  ClearPolarityBit();
}

/*
  Clear polarity bit

  Polarity is synchronous mode stuff.

  For async mode this bit should be set to zero.
*/
void Freetown::ClearPolarityBit()
{
  // Value 0. Register 3, offset 0

  Registers->Polarity = 0;
}

// Set one stop bit in a frame
void Freetown::SetOneStopBit()
{
  // Value 0. Register 3, offset 3

  Registers->StopBits = 0;
}

// Set no parity
void Freetown::SetNoParity()
{
  // Value 00. Register 3, offset 4

  Registers->Parity = 0;
}

// Set frame size to 8 bits
void Freetown::Set8BitsFrame()
{
  /*
    Value 011

    Highest bit lives in register 2, offset 2.
    Other two are in register 3 offset 1.
  */

  Registers->FrameSize_12 = 3;
  Registers->FrameSize_3 = 0;
}

// Disable interrupt when data frame received
void Freetown::DisableOnReceiveCompleteInterrupt()
{
  // Value 0. Register 2, offset 7

  Registers->EnableOnReceiveCompleteInterrupt = false;
}

// Disable interrupt when data frame sent
void Freetown::DisableOnTransmitCompleteInterrupt()
{
  // Value 0. Register 2, offset 6

  Registers->EnableOnTransmitCompleteInterrupt = false;
}

// Disable interrupt when no data
void Freetown::DisableOnEmptyBufferInterrupt()
{
  // Value 0. Register 2, offset 5

  Registers->EnableOnEmptyBufferInterrupt = false;
}

// Disable receiver
void Freetown::DisableReceiver()
{
  // Value 0. Register 2, offset 4

  Registers->EnableReceiver = false;
}

// Enable receiver
void Freetown::EnableReceiver()
{
  // Value 1. Register 2, offset 4

  Registers->EnableReceiver = true;
}

// Disable transmitter
void Freetown::DisableTransmitter()
{
  // Value 0. Register 2, offset 3

  Registers->EnableTransmitter = false;
}

// Enable transmitter
void Freetown::EnableTransmitter()
{
  // Value 1. Register 2, offset 3

  Registers->EnableTransmitter = true;
}

// Return true when transmitter is idle
TBool Freetown::ReadyToTransmit()
{
  // Register 1, offset 5. Read-only

  return Registers->ReadyToTransmit;
}

// Return true when there is data in receive buffer
TBool Freetown::ReceivedByte()
{
  // Register 1, offset 7. Read-only

  return Registers->ReceivedByte;
}

// Return true if frame is received with errors
TBool Freetown::FrameHasErrors()
{
  // Value should be not 000. Register 1, offset 2

  return (Registers->FrameHasErrors != 0);
}

// Put byte to transceiver buffer
void Freetown::Buffer_PutByte(
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
void Freetown::Buffer_ExtractByte(
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
