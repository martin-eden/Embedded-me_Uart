// UART access. Backend functions

/*
  Author: Martin Eden
  Last mod.: 2024-10-29
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

#include <me_MemorySegment.h> // your time to shine, sweetie!
#include <me_Bits.h>

using namespace me_Uart;

using
  me_MemorySegment::TMemorySegment,
  me_MemorySegment::Freetown::FromAddrSize,
  me_Bits::GetBit,
  me_Bits::SetBit;

// ( Freetown

const TMemorySegment UartStatusReg_1 =
  FromAddrSize(192, 1);
const TMemorySegment UartStatusReg_2 =
  FromAddrSize(193, 1);
const TMemorySegment UartStatusReg_3 =
  FromAddrSize(194, 1);
const TMemorySegment Counter_Limit =
  FromAddrSize(196, 2);
const TMemorySegment UartBuffer =
  FromAddrSize(198, 1);

// Set bit duration. Custom unit. Not all durations can be set
TBool Freetown::SetBitDuration_ut(
  TUint_2 BitDuration_ut
)
{
  // Memory value is 14-bit word

  /*
    We're setting limit value for (0, N) "for" loop.

    It will always run at least once.
  */

  if (BitDuration_ut == 0)
    return false;

  TUint_2 Limit = BitDuration_ut - 1;

  // Max value we can store
  TUint_2 MaxLimit = (1 << 14) - 1;

  if (Limit > MaxLimit)
    return false;

  /*
    Hardware magic occurs at writing low byte of counter.
    So we're writing high byte first.
  */
  Counter_Limit.Bytes[1] = (Limit >> 8) & 0xFF;
  Counter_Limit.Bytes[0] = Limit & 0xFF;

  return true;
}

// Use normal transceiver speed
void Freetown::SetNormalSpeed()
{
  // Value 0. Register 1, offset 1

  const TUint_1 BitOffs = 1;

  SetBit(&UartStatusReg_1.Bytes[0], BitOffs, false);
}

// Use double transceiver speed
void Freetown::SetDoubleSpeed()
{
  // Value 1. Register 1, offset 1

  const TUint_1 BitOffs = 1;

  SetBit(&UartStatusReg_1.Bytes[0], BitOffs, true);
}

// Set asynchronous UART mode
void Freetown::SetAsyncMode()
{
  // Value 00. Register 3, offset 6

  const TUint_1 BitfieldOffs = 6;

  UartStatusReg_3.Bytes[0] =
    UartStatusReg_3.Bytes[0] & ~(0x03 << BitfieldOffs);

  ClearPolarityBit();
}

/*
  Clear polarity bit

  Polarity is synchronous mode stuff. For async mode
  this bit should be set to zero.
*/
void Freetown::ClearPolarityBit()
{
  // Value 0. Register 3, offset 0

  const TUint_1 BitOffs = 0;

  SetBit(&UartStatusReg_3.Bytes[0], BitOffs, false);
}

// Set one stop bit in a frame
void Freetown::SetOneStopBit()
{
  // Value 0. Register 3, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(&UartStatusReg_3.Bytes[0], BitOffs, false);
}

// Set no parity
void Freetown::SetNoParity()
{
  // Value 00. Register 3, offset 4

  const TUint_1 BitfieldOffs = 4;

  UartStatusReg_3.Bytes[0] =
    UartStatusReg_3.Bytes[0] & ~(0x03 << BitfieldOffs);
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

  SetBit(&UartStatusReg_2.Bytes[0], Bit3_BitOffs, false);

  UartStatusReg_3.Bytes[0] =
    UartStatusReg_3.Bytes[0] | (0x03 << BitfieldOffs);
}

// Disable interrupt when data frame received
void Freetown::DisableOnReceiveCompleteInterrupt()
{
  // Value 0. Register 2, offset 7

  const TUint_1 BitOffs = 7;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, false);
}

// Disable interrupt when data frame sent
void Freetown::DisableOnTransmitCompleteInterrupt()
{
  // Value 0. Register 2, offset 6

  const TUint_1 BitOffs = 6;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, false);
}

// Disable interrupt when no data
void Freetown::DisableOnEmptyBufferInterrupt()
{
  // Value 0. Register 2, offset 5

  const TUint_1 BitOffs = 5;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, false);
}

// Disable receiver
void Freetown::DisableReceiver()
{
  // Value 0. Register 2, offset 4

  const TUint_1 BitOffs = 4;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, false);
}

// Enable receiver
void Freetown::EnableReceiver()
{
  // Value 1. Register 2, offset 4

  const TUint_1 BitOffs = 4;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, true);
}

// Enable transmitter
void Freetown::EnableTransmitter()
{
  // Value 1. Register 2, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, true);
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

  UartBuffer.Bytes[0] = Data;
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

  *Data = UartBuffer.Bytes[0];
}

// Return true when transmitter is idle
TBool Freetown::ReadyToTransmit()
{
  // Register 1, offset 5. Read-only

  const TUint_1 BitOffs = 5;

  return GetBit(UartStatusReg_1.Bytes[0], BitOffs);
}

// Return true when there is data in receive buffer
TBool Freetown::ReceivedByte()
{
  // Register 1, offset 7. Read-only

  const TUint_1 BitOffs = 7;

  return GetBit(UartStatusReg_1.Bytes[0], BitOffs);
}

// Return true if there is parity error (wrong xor bit) for received data
TBool Freetown::Receive_IsParityError()
{
  // Register 1, offset 2. Read-only

  const TUint_1 BitOffs = 2;

  return GetBit(UartStatusReg_1.Bytes[0], BitOffs);
}

// Return true when there is frame error (too much bits) for received data
TBool Freetown::Receive_IsFrameError()
{
  // Register 1, offset 4. Read-only

  const TUint_1 BitOffs = 4;

  return GetBit(UartStatusReg_1.Bytes[0], BitOffs);
}

// Return true when there is data overrun (we got bytes, but noone read them)
TBool Freetown::Receive_IsDataOverrun()
{
  // Register 1, offset 3. Read-only

  const TUint_1 BitOffs = 3;

  return GetBit(UartStatusReg_1.Bytes[0], BitOffs);
}

// ) Freetown

/*
  2024-10-25
  2024-10-26
  2024-10-27
*/
