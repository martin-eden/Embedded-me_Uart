// USART registers layout

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
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
    TUint_1 StopBits : 1;
    TUint_1 Parity : 2;
    TUint_1 TransceiverMode : 2;

    // Byte 4 (Reserved)
    TUint_1 : 8;

    // Bytes 5 and 6 (UBRR0L and URR0H)
    TBitDuration BitDuration;

    // Byte 7 (UDR0)
    TUint_1 Buffer : 8;
  };

  // Mapping to address. Actually done in "me_Uart_Bare.cpp"
  extern TUart * Uart;
}

/*
  2024-10 # # # #
  2024-11 # # # #
  2024-12 #
  2025-07-13
*/
