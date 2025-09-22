// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2025-09-22
*/

#pragma once

#include <me_BaseTypes.h>
#include <me_Switch.h>

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
  class TReceivedDataInterrupt : public me_Switch::TSwitch
  {
    protected:
      void Get(TBool *) override;
      void Set(TBool) override;
  };

  // Interrupt when data frame sent
  class TSentDataInterrupt : public me_Switch::TSwitch
  {
    protected:
      void Get(TBool *) override;
      void Set(TBool) override;
  };

  // Interrupt when transmitter is ready for new data
  class TReadyToSendInterrupt : public me_Switch::TSwitch
  {
    protected:
      void Get(TBool *) override;
      void Set(TBool) override;
  };

  class TReceiverSwitch : public me_Switch::TSwitch
  {
    protected:
      void Get(TBool *) override;
      void Set(TBool) override;
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

  class TTransmitterSwitch : public me_Switch::TSwitch
  {
    protected:
      void Get(TBool *) override;
      void Set(TBool) override;
  };

  // Transmitter
  class TTransmitter
  {
    public:
      TTransmitterSwitch Switch;
      TBool IsReady();
      void Put(TUint_1 Data);
      TBool IsSent();
      void ClearSentFlag();
      TReadyToSendInterrupt IsReadyInterrupt;
      TSentDataInterrupt FrameSentInterrupt;
  };
}

/*
  2024-10 # # # # # # # # #
  2025-07-13
  2025-09-05
*/
