// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-10-26
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

TBool TUartChannel::Init(
  TUint_4 Speed_Bps
)
{
  /*
    It's not that hard.

    As usually there is idle cycle "(for i = 0, N)()" to keep line
    state long enough.

    That <N> value is amount of 16-clocks ticks (or 8-clocks ticks for
    double speed mode).

    That <N> value is stored in 14 bits, so max(N) = 4095.
    So maximum delay is 4096 * 16-clock ticks. 65536 ticks.
    For 16 MHz it is near 4 ms. (4096 us to be precise.)

    So lowest baud rate is near 250 bps (1000 / 4). Highest baud rate
    is 1 Mbps (or 100 KB / s). Will transfer megabyte in ten seconds.
    Not enough for porn but not bad actually.
  */

  const TUint_4 MaxBps = F_CPU / 16;

  // *_ucsra = 0;

  /*
    We can fit (MaxBps / SpeedBps) chunks

    But we will round counter limit to nearest integer.
    So (MaxBps + (SpeedBps/2)) / SpeedBps).

    And we will subtract 1, cause zero is cool.
    (And cause idle loop is from 0.)
  */
  TUint_4 BaudCounter_Limit =
    (MaxBps + (Speed_Bps / 2)) / Speed_Bps - 1;

  // Max value we can store for idle counter
  TUint_2 CounterCapacity = (1 << 14) - 1;

  // Speed is too slow. We can't handle slow speeds, lol
  if (BaudCounter_Limit > CounterCapacity)
    return false;

  // 14-bit value to store counter max value
  const TMemorySegment Counter_Limit =
    FromAddrSize(196, 2);

  /*
    Hardware magic occurs at writing low byte of counter.
    So we're writing high byte first.
  */
  Counter_Limit.Bytes[1] = (BaudCounter_Limit >> 8) & 0xFF;
  Counter_Limit.Bytes[0] = BaudCounter_Limit & 0xFF;

  Freetown::SetAsyncMode();
  Freetown::SetOneStopBit();
  Freetown::SetNoParity();
  Freetown::Set8BitsFrame();

  Freetown::DisableOnReceiveCompleteInterrupt();
  Freetown::DisableOnTransmitCompleteInterrupt();
  Freetown::DisableOnEmptyBufferInterrupt();

  Freetown::DisableReceiver();
  Freetown::EnableTransmitter();

  return true;
}

TBool TUartChannel::SendByte(
  TUint_1 Value
)
{
  return Freetown::TransmitFrame(Value);
}

// ( Freetown
const TMemorySegment StatusReg =
  FromAddrSize(95, 1);
const TMemorySegment UartStatusReg_1 =
  FromAddrSize(192, 1);
const TMemorySegment UartStatusReg_2 =
  FromAddrSize(193, 1);
const TMemorySegment UartStatusReg_3 =
  FromAddrSize(194, 1);
const TMemorySegment UartData =
  FromAddrSize(198, 1);

// Stolen from [avr/interrupt.h]
# define cli()  __asm__ __volatile__ ("cli" ::: "memory")

// Set asynchronous UART mode
void Freetown::SetAsyncMode()
{
  // Value 00. Register 3, offset 6

  const TUint_1 BitfieldOffs = 6;

  UartStatusReg_3.Bytes[0] = UartStatusReg_3.Bytes[0] & (0x03 << BitfieldOffs);

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

  UartStatusReg_3.Bytes[0] = UartStatusReg_3.Bytes[0] & ~(0x03 << BitfieldOffs);
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

  UartStatusReg_3.Bytes[0] = UartStatusReg_3.Bytes[0] | (0x03 << BitfieldOffs);
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

// Enable transmitter
void Freetown::EnableTransmitter()
{
  // Value 1. Register 2, offset 3

  const TUint_1 BitOffs = 3;

  SetBit(&UartStatusReg_2.Bytes[0], BitOffs, true);
}

/*
  Transmit data frame up to 8 bits in size.

  Data frame size can be 5, 6, 7, 8, and 9 bits.
  Transmitting 9-bit frames requires more work and not needed.
*/
TBool Freetown::TransmitFrame(
  TUint_1 Data
)
{
  if (!ReadyToTransmit())
    return false;

  TUint_1 OrigSreg = StatusReg.Bytes[0];
  cli();

  UartData.Bytes[0] = Data;

  StatusReg.Bytes[0] = OrigSreg;

  return true;
}

// Return true when transmitter is idle
TBool Freetown::ReadyToTransmit()
{
  // 1 - buffer is empty. Register 1, offset 5.

  const TUint_1 TransmitBufferIsEmpty_BitOffs = 5;

  return GetBit(UartStatusReg_1.Bytes[0], TransmitBufferIsEmpty_BitOffs);
}

// ) Freetown

/*
  2024-10-25
*/
