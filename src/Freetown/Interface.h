// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart_Freetown
{
  // Mode setter
  class TModeSetter
  {
    public:
      // Set asynchronous mode
      void SetAsyncMode();

    protected:
      void ClearPolarityBit();
  };

  // Frame setter
  class TFrameSetter
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
  class TSpeedSetter
  {
    public:
      // Set speed in baud
      TBool SetSpeed(TUint_4 Speed_Bps);
      // Estimate real speed
      TBool GetSpeed(TUint_4 * Speed_Bps);

    protected:
      void SetBitDuration_ut(TUint_2 BitDuration);
      void SetNormalSpeed();
      void SetDoubleSpeed();
  };

  // Interrupt when data frame received
  class TReceivedDataInterrupt
  {
    public:
      void On();
      void Off();
  };

  // Interrupt when data frame sent
  class TSentDataInterrupt
  {
    public:
      void On();
      void Off();
  };

  // Interrupt when transmitter is ready for new data
  class TReadyToSendInterrupt
  {
    public:
      void On();
      void Off();
  };

  // Receiver
  class TReceiver
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
  class TTransmitter
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

/*
  2024-10 # # # #
  2024-11 # # # #
  2024-12 #
  2025-07-13
*/
