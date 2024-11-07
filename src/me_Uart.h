// UART access

/*
  Author: Martin Eden
  Last mod.: 2024-11-06
*/

/*
  Here is unsophisticated design. Three core functions.

  We are not using external buffers for transmission and receiving.
*/

/*
  ASCII-art documentation for kids and adults

                         Transmitter
  -----------------------------------------------------------
       |                 Data                   |
   On  |     7   6   5   4   3   2   1   0      | (*) Ready
   [x] |    [ ] [ ] [ ] [ ] [ ] [ ] [ ] [ ]     |  ______
    |  |     |   |   |   |   |   |   |   |      | | Send |
   [ ] |    [x] [x] [x] [x] [x] [x] [x] [x]     | `~~~~~~'
       |                                        |
  -----------------------------------------------------------

                         Receiver
  -----------------------------------------------------------
       |                 Data                   |
   On  |                                        | (*) Ready
   [x] |     7   6   5   4   3   2   1   0      | (o) Errors
    |  |    (o) (o) (o) (o) (o) (o) (o) (o)     |  ______
   [ ] |                                        | | Next |
       |                                        | `~~~~~~'
  -----------------------------------------------------------

                      Data format setter

       Bits       |         Parity       | Double stop
  --------------  | -------------------- | ------------
                  |   On    |  Invert    |     On
      |           |   [ ]   |   [ ]      |     [ ]
    9-8-7 6 5     |    |    |    |       |      |
    | ^ | | |     |  -[x]-  |   [x]      |    -[x]-
      |           |    |    |            |      |
  --------------  | -------------------- |-------------
*/

#pragma once

#include <me_BaseTypes.h>

namespace me_Uart
{
  // Set-up for given speed (no parity, 8 data bits, 1 stop bit)
  TBool Init(TUint_4 Speed);

  // Send byte
  void SendByte(TUint_1 Value);

  // Receive byte
  TBool ReceiveByte(TUint_1 * Value);

  // Await byte
  void AwaitByte(TUint_1 * Value);

  namespace Freetown
  {
    // ( "SetSpeed" module

    // Set transceiver speed
    TBool SetSpeed(TUint_4 Speed_Bps);

    // Calculate bit duration in hardware time units
    TUint_4 CalculateBitDuration_ut(TUint_4 Speed_Bps, TBool UseDoubleSpeed);

    // Set bit duration. Not all durations can be set
    TBool SetBitDuration_ut(TUint_2 BitDuration_ut);

    // Use normal transceiver speed
    void SetNormalSpeed();

    // Use double transceiver speed
    void SetDoubleSpeed();

    // )

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

    // Enable receiver
    void EnableReceiver();

    // Disable transmitter
    void DisableTransmitter();

    // Enable transmitter
    void EnableTransmitter();

    // Return true when transmitter is idle
    TBool ReadyToTransmit() __attribute__((optimize("O0")));

    // Put byte to transceiver buffer
    void Buffer_PutByte(TUint_1 Data);

    // Extract byte from transceiver buffer
    void Buffer_ExtractByte(TUint_1 * Data);

    // Return true when there is data in receive buffer
    TBool ReceivedByte() __attribute__((optimize("O1")));

    // Return true if frame is received with errors
    TBool FrameHasErrors() __attribute__((optimize("O1")));
  }
}

/*
  2024-10-25
  2024-10-26
  2024-10-27
  2024-10-29
  2024-11-01
*/
