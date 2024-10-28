// UART access. Backend functions

/*
  Author: Martin Eden
  Last mod.: 2024-10-29
*/

/*
  My lovely anarchistic society!

  Lot of functions. But they do no call each other.

  Two mathematical functions. They care about values in SI.

  One calculates bit duration is microseconds. Other have to convert
  microseconds to hardwired loop "ticks".

  All others care only about reading/writing designated memory location.
  I could write "sbi" or direct assignments. I chose not to. Names are
  better than comments to uncomprehensible code. Comments are just poor
  man's names.
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

// Calculate bit duration in microseconds
TUint_4 Freetown::CalculateBitDuration_us(
  TUint_4 Speed_Bps
)
{
  /*
    Explaining formulae

      1 second = 1000000 microseconds

      115200 bits are transferred in that duration

      What is duration of one bit?

      1000000 / 115200 ~= 8.68 ~= 8

      8 * 115200 = 921600 // 8% error
      9 * 115200 = 1036800 // 3 % error

      So we'll do rounding by adding half of 115200 to 1000000:

      1057600 / 115200 ~= 9.18 ~= 9
  */

  return
    (1000000 + Speed_Bps / 2) / Speed_Bps;
}

// Set bit duration. Not all durations can be set
TBool Freetown::SetBitDuration_us(
  TUint_4 BitDuration_us
)
{
  /*
    Things are more hairy at hardware level.

    There is idle cycle (for i = 0, N)(wait 16 ticks).
    It's needed to keep line stable for that time.
    <N> is counter limit value that is stored in
    (God forgive me for using that names!) in
    UBRR0H and UBRR0L.

    That's just 14-bits word for <N>.

    For "double speed" wait is 8 ticks. But we're sticking to
    "normal" speed.

    We're converting duration in microseconds to that counter
    value and writing it.

    (N + 1) * 16 * TickDuration_us == BitDuration_us
    N == (BitDuration_us / (16 * TickDuration_us)) - 1

    TickDuration_us = 1 / CpuTicksPerSecond * 1000000 // = 0.0625 us for 16 MHz

    Too bad for integer calculations. So we're wrapping all in one
    expression:

      N + 1 = BitDuration_us / (16 * (1 / F_CPU) * 1000000)
        = BitDuration_us / (16000000 / F_CPU)
        = BitDuration_us * F_CPU / 16000000
  */

  const TUint_1 TicksInCycle = 16;

  TUint_4 CounterValue =
    BitDuration_us * F_CPU / (TicksInCycle * 1000000) - 1;

  // Max value we can store for idle counter
  TUint_2 CounterCapacity = (1 << 14) - 1;

  // Speed is too slow. We can't handle slow speeds, lol
  if (CounterValue > CounterCapacity)
    return false;

  /*
    Hardware magic occurs at writing low byte of counter.
    So we're writing high byte first.
  */
  Counter_Limit.Bytes[1] = (CounterValue >> 8) & 0xFF;
  Counter_Limit.Bytes[0] = CounterValue & 0xFF;

  return true;
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

/*
  Put byte to transceiver buffer

  Writing to this memory location updates flags and
  initiates hardware transmitter.
*/
void Freetown::Buffer_PutByte(
  TUint_1 Data
)
{
  UartBuffer.Bytes[0] = Data;
}

/*
  Extract byte from transceiver buffer

  Reading this memory location updates flags and
  initiates hardware receiver.
*/
void Freetown::Buffer_ExtractByte(
  TUint_1 * Data
)
{
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
