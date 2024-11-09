// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2024-11-08
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  namespace Freetown
  {
    struct TRegister
    {
      TBool Unused_1_1 : 1;
      TBool UseDoubleSpeed : 1;
      volatile TUint_1 FrameHasErrors : 3;
      volatile TBool ReadyToTransmit : 1;
      volatile TBool Transmitted : 1;
      volatile TBool Received : 1;

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

    union TCounterLimit
    {
      TUint_2 Value : 12;
      struct
      {
        TUint_1 Value_LowByte : 8;
        TUint_1 Value_HighByte : 4;
      };
    };

    // Mapped device memory
    class TBareUart
    {
      protected:
        TRegister * Register = (TRegister *) 192;
        TCounterLimit * CounterLimit = (TCounterLimit *) 196;
        TUint_1 * Buffer = (TUint_1 *) 198;
    };

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

    // Transmitter
    class TTransmitter : protected TBareUart
    {
      public:
        void On();
        void Off();
        TBool IsReady();
        void Put(TUint_1 Data);
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
    };

    // Disable interrupt when data frame received
    void DisableOnReceiveCompleteInterrupt();

    // Disable interrupt when data frame sent
    void DisableOnTransmitCompleteInterrupt();

    // Disable interrupt when no data
    void DisableOnEmptyBufferInterrupt();
  }
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
  2024-11-08
*/
