// USART registers layout

/*
  Author: Martin Eden
  Last mod.: 2026-11-21
*/

/*
  Storage format

  Below is LISP-like pseudocode. That's bits tree and harware
  behavior notes.

  bits_tree
  (
    1537
    (
      02 01 UseDoubleSpeed
      03 03 FrameHasErrors
      06 01 ReadyToSend
      07 01 Sent
      08 01 Received
      11 01 FrameSize_3
      12 01 EnableTransmitter
      13 01 EnableReceiver
      14 01 EnableOnReadyToSendInterrupt
      15 01 EnableOnSentInterrupt
      16 01 EnableOnReceivedInterrupt
      17 01 Sync_Polarity
      18 02 FrameSize_12
      20 01 UseTwoStopBits
      21 02 Parity
      23 02 TransceiverMode
      33 12 BitDuration
      49 08 Buffer
    )
    UART
  )
  (* <Received> flag is cleared by reading <Buffer> )
  (* <Sent> flag is cleared by writing 1 )
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart_Bare
{
  // Baudrate divisor (or bit loop counter limit or bit duration)
  union TBitDuration
  {
    struct
    {
      TUint_2 Duration : 12;
      TUint_1 : 4;
    };
    struct
    {
      TUint_1 Duration_LowByte : 8;
      TUint_1 Duration_HighByte : 4;
      TUint_1 : 4;
    };
  };

  // Layout of USART registers
  struct TUart
  {
    // Byte 1 (UCSR0A)
    TUint_1 : 1;
    TBool UseDoubleSpeed : 1;
    volatile TUint_1 FrameHasErrors : 3;
    volatile TBool ReadyToSend : 1;
    volatile TBool Sent : 1;
    volatile TBool Received : 1;

    // Byte 2 (USCR0B)
    TUint_1 : 1;
    TUint_1 : 1;
    TUint_1 FrameSize_3 : 1;
    TBool EnableTransmitter : 1;
    TBool EnableReceiver : 1;
    TBool EnableOnReadyToSendInterrupt : 1;
    TBool EnableOnSentInterrupt : 1;
    TBool EnableOnReceivedInterrupt : 1;

    // Byte 3 (UCSR0C)
    TUint_1 Sync_Polarity : 1;
    TUint_1 FrameSize_12 : 2;
    TBool UseTwoStopBits : 1;
    TUint_1 Parity : 2;
    TUint_1 TransceiverMode : 2;

    // Byte 4 (Reserved)
    TUint_1 : 8;

    // Bytes 5 and 6 (UBRR0L and URR0H)
    TBitDuration BitDuration;

    // Byte 7 (UDR0)
    volatile TUint_1 Buffer : 8;
  };

  // Mapping to address
  extern TUart * Uart;
}

/*
  2024 # # # # # # # #
  2025 #
  2026-03-02
*/
