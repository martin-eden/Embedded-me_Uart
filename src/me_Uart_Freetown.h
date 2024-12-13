// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2024-12-13
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  namespace Freetown
  {
    // Baudrate divisor (or bit loop counter limit)
    union TBitDuration
    {
      TUint_2 Value : 12;
      struct
      {
        TUint_1 Value_LowByte : 8;
        TUint_1 Value_HighByte : 4;
      };
      TUint_1 Unused : 4;
    };

    /*
      Memory map of USART registers

      For ATmega328/P USART state occupies memory from 0xC0 to 0xC6.
      Seven bytes.
    */
    struct TUartState
    {
      // Byte 1 (UCSR0A)
      TBool Unused_1_1 : 1;
      TBool UseDoubleSpeed : 1;
      volatile TUint_1 FrameHasErrors : 3;
      volatile TBool ReadyToSend : 1;
      volatile TBool Sent : 1;
      volatile TBool Received : 1;

      // Byte 2 (USCR0B)
      TBool Unused_2_1 : 1;
      TBool Unused_2_2 : 1;
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
      TUint_1 Unused_4;

      // Bytes 5 and 6 (UBRR0L and URR0H)
      TBitDuration BitDuration;

      // Byte 7 (UDR0)
      TUint_1 Buffer;
    };

    // Mapped state
    class TBareUart
    {
      protected:
        TUartState * Uart = (TUartState *) 192;
    };

    // Mode setter
    class TModeSetter : protected TBareUart
    {
      public:
        // Set asynchronous mode
        void SetAsyncMode();

      protected:
        void ClearPolarityBit();
    };

    // Frame setter
    class TFrameSetter : protected TBareUart
    {
      public:
        // Set frame size to 8 bits
        void Set8BitsFrame();
        // Set no parity
        void SetNoParity();
        // Set one stop bit in a frame
        void SetOneStopBit();
    };

    // Speed setter
    class TSpeedSetter : protected TBareUart
    {
      public:
        // Set speed in baud
        TBool SetSpeed(TUint_4 Speed_Bps);

      protected:
        TUint_4 CalculateBitDuration_ut(TUint_4 Speed_Bps, TBool UseDoubleSpeed);
        void SetNormalSpeed();
        void SetDoubleSpeed();
        TBool SetBitDuration_ut(TUint_2 BitDuration_ut);
    };

    // Interrupt when data frame received
    class TReceivedDataInterrupt : protected TBareUart
    {
      public:
        void On();
        void Off();
    };

    // Interrupt when data frame sent
    class TSentDataInterrupt : protected TBareUart
    {
      public:
        void On();
        void Off();
    };

    // Interrupt when transmitter is ready for new data
    class TReadyToSendInterrupt : protected TBareUart
    {
      public:
        void On();
        void Off();
    };

    // Receiver
    class TReceiver : protected TBareUart
    {
      public:
        void On();
        void Off();
        TBool HasData();
        TBool AreErrors();
        TUint_1 Get();
        TReceivedDataInterrupt HasDataInterrupt;
    };

    // Transmitter
    class TTransmitter : protected TBareUart
    {
      public:
        void On();
        void Off();
        TBool IsReady();
        void Put(TUint_1 Data);
        TReadyToSendInterrupt IsReadyInterrupt;
        TSentDataInterrupt FrameSentInterrupt;
    };
  }
}

/*
  2024-10 ####
  2024-11 ####
  2024-12-13
*/
