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

#include <me_Bits.h>

using namespace me_Uart;

using
  me_Bits::GetBit,
  me_Bits::SetBit;

// ( Freetown

TUint_1 * UartStatusReg_1 = (TUint_1 *) 192;
TUint_1 * UartStatusReg_2 = (TUint_1 *) 193;
TUint_1 * UartStatusReg_3 = (TUint_1 *) 194;

TUint_1 * UartBuffer = (TUint_1 *) 198;

// Set asynchronous UART mode
void Freetown::SetAsyncMode()
{
  // Value 00. Register 3, offset 6

  const TUint_1 BitfieldOffs = 6;

  *UartStatusReg_3 =
    *UartStatusReg_3 & ~(0x03 << BitfieldOffs);

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

  const TUint_1 BitOffs = 0;

  SetBit(UartStatusReg_3, BitOffs, false);
}

// Set one stop bit in a frame
void Freetown::SetOneStopBit()
{
  // Value 0. Register 3, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(UartStatusReg_3, BitOffs, false);
}

// Set no parity
void Freetown::SetNoParity()
{
  // Value 00. Register 3, offset 4

  const TUint_1 BitfieldOffs = 4;

  *UartStatusReg_3 =
    *UartStatusReg_3 & ~(0x03 << BitfieldOffs);
}

// Set frame size to 8 bits
void Freetown::Set8BitsFrame()
{
  /*
    Value 011

    Highest bit lives in register 2, offset 2.
    Other two are in register 3 offset 1.
  */
  const TUint_1 Bit3_BitOffs = 2;
  const TUint_1 BitfieldOffs = 1;

  SetBit(UartStatusReg_2, Bit3_BitOffs, false);

  *UartStatusReg_3 =
    *UartStatusReg_3 | (0x03 << BitfieldOffs);
}

// Disable interrupt when data frame received
void Freetown::DisableOnReceiveCompleteInterrupt()
{
  // Value 0. Register 2, offset 7

  const TUint_1 BitOffs = 7;

  SetBit(UartStatusReg_2, BitOffs, false);
}

// Disable interrupt when data frame sent
void Freetown::DisableOnTransmitCompleteInterrupt()
{
  // Value 0. Register 2, offset 6

  const TUint_1 BitOffs = 6;

  SetBit(UartStatusReg_2, BitOffs, false);
}

// Disable interrupt when no data
void Freetown::DisableOnEmptyBufferInterrupt()
{
  // Value 0. Register 2, offset 5

  const TUint_1 BitOffs = 5;

  SetBit(UartStatusReg_2, BitOffs, false);
}

// Disable receiver
void Freetown::DisableReceiver()
{
  // Value 0. Register 2, offset 4

  const TUint_1 BitOffs = 4;

  SetBit(UartStatusReg_2, BitOffs, false);
}

// Enable receiver
void Freetown::EnableReceiver()
{
  // Value 1. Register 2, offset 4

  const TUint_1 BitOffs = 4;

  SetBit(UartStatusReg_2, BitOffs, true);
}

// Disable transmitter
void Freetown::DisableTransmitter()
{
  // Value 0. Register 2, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(UartStatusReg_2, BitOffs, false);
}

// Enable transmitter
void Freetown::EnableTransmitter()
{
  // Value 1. Register 2, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(UartStatusReg_2, BitOffs, true);
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

// Return true when transmitter is idle
TBool Freetown::ReadyToTransmit()
{
  // Register 1, offset 5. Read-only

  const TUint_1 BitOffs = 5;

  return GetBit(*UartStatusReg_1, BitOffs);
}

// Return true when there is data in receive buffer
TBool Freetown::ReceivedByte()
{
  // Register 1, offset 7. Read-only

  const TUint_1 BitOffs = 7;

  return GetBit(*UartStatusReg_1, BitOffs);
}

// Return true if frame is received with errors
TBool Freetown::FrameHasErrors()
{
  // Value should be 000. Register 1, offset 2

  /*
    There are three error flags: bad parity, wrong length and
    buffer overflow. Originally there was getter for each flag.
    But at 250K making three calls taking too long. So this
    union function.

    Over 250K speed blocker is timer interrupt. Without interrupts
    transmitter works at 1M.
  */

  const TUint_1 BitfieldOffs = 2;

  const TUint_1 Mask = (0x7 << BitfieldOffs);

  return (*UartStatusReg_1 & Mask) != 0;
}

// ) Freetown

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
*/
