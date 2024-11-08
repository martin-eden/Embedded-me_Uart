// UART registers accessors

/*
  Author: Martin Eden
  Last mod.: 2024-11-08
*/

/*
  Functional diagram for kids and adults

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

                           Speed setter

     |
     | *
     |
  1M |  *                                                [ ]
     |                                                    |  Speed x 2
     |   *                                               [x]
     |
     |
     |      *
     |
     |          *
     |              *
     |                    *
     |                             *
     |                                           *
     ---|-----^----------------------------------|----
        0     |                                4095
           Duration
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
      TBool Unused_1_7 : 1;
      volatile TBool ReceivedByte : 1;

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

    // ( "SetSpeed" module

    // Set transceiver speed
    TBool SetSpeed(TUint_4 Speed_Bps);

    // Calculate bit duration in hardware time units
    TUint_4 CalculateBitDuration_ut(TUint_4 Speed_Bps, TBool UseDoubleSpeed);

    // Set bit duration. Not all durations can be set
    TBool SetBitDuration_ut(TUint_2 BitDuration_ut);

    // )

    // Use normal transceiver speed
    void SetNormalSpeed();

    // Use double transceiver speed
    void SetDoubleSpeed();

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
    TBool ReadyToTransmit();

    // Put byte to transceiver buffer
    void Buffer_PutByte(TUint_1 Data);

    // Extract byte from transceiver buffer
    void Buffer_ExtractByte(TUint_1 * Data);

    // Return true when there is data in receive buffer
    TBool ReceivedByte();

    // Return true if frame is received with errors
    TBool FrameHasErrors();
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
