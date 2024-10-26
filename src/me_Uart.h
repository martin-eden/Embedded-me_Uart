// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-10-25
*/

/*
  I have no clear vision what it will become.
  But I know I want SendByte() and GetByte().
  And I know I can do this.

  -- Martin, 2024-10-25
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  class TUartChannel
  {
    private:

    public:
      TBool Init(TUint_4 Speed);
      TBool SendByte(TUint_1 Value);
  };

  namespace Freetown
  {
    // Set asynchronous UART mode
    void SetAsyncMode();

    // Clear polarity bit. Polarity is synchronous mode stuff
    void ClearPolarityBit();

    // Set one stop bit in a frame
    void SetOneStopBit();

    // Set no parity
    void SetNoParity();

    // Set frame size to 8 bits
    void Set8BitsFrame();

    // Disable interrupt when data frame received
    void DisableOnReceiveCompleteInterrupt();

    // Disable interrupt when data frame sent
    void DisableOnTransmitCompleteInterrupt();

    // Disable interrupt when no data
    void DisableOnEmptyBufferInterrupt();

    // Disable receiver
    void DisableReceiver();

    // Enable transmitter
    void EnableTransmitter();

    // Return true when transmitter is idle
    TBool ReadyToTransmit();

    // Transmit data frame up to 8 bits in size
    TBool TransmitFrame(TUint_1 Data);
  }
}

/*
  2024-10-25
  2024-10-26
*/
