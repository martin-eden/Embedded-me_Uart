// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2025-10-22
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
  };

  // [Internal] Switch implementation without virtual methods
  class TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Interrupt when data frame received
  class TReceivedDataInterrupt : public TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Interrupt when data frame sent
  class TSentDataInterrupt : public TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Interrupt when transmitter is ready for new data
  class TReadyToSendInterrupt : public TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Receiver switch
  class TReceiverSwitch : public TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Receiver
  class TReceiver
  {
    public:
      TReceiverSwitch Switch;
      TBool HasData();
      TBool AreErrors();
      TUint_1 Get();
      TReceivedDataInterrupt HasDataInterrupt;
  };

  // Transmitter switch
  class TTransmitterSwitch : public TSwitch
  {
    public:
      TBool IsOn();
      void On();
      void Off();
  };

  // Transmitter
  class TTransmitter
  {
    public:
      TTransmitterSwitch Switch;
      TBool IsReady();
      void Send(TUint_1 Data);
      TBool IsSent();
      void ClearSentFlag();
      TReadyToSendInterrupt IsReadyInterrupt;
      TSentDataInterrupt FrameSentInterrupt;
  };
}

/*
  2024 # # # # # # # # #
  2025 # # # #
  2025-10-22
*/
