// UART access. Frontend functions

/*
  Author: Martin Eden
  Last mod.: 2025-08-29
*/

#include <me_Uart.h>

#include <me_BaseTypes.h>

#include "Freetown/Interface.h"

using namespace me_Uart;

/*
  Set-up for given speed

  No parity, 8 data bits, 1 stop bit.
*/
TBool me_Uart::Init(
  TUint_4 Speed_Bps
)
{
  me_Uart_Freetown::TReceiver Receiver;
  me_Uart_Freetown::TTransmitter Transmitter;
  me_Uart_Freetown::TModeSetter ModeSetter;
  me_Uart_Freetown::TFrameSetter FrameSetter;
  me_Uart_Freetown::TSpeedSetter SpeedSetter;

  Receiver.Off();
  Transmitter.Off();

  ModeSetter.SetAsyncMode();

  FrameSetter.SetOneStopBit();
  FrameSetter.SetNoParity();
  FrameSetter.Set8BitsFrame();

  if (!SpeedSetter.SetSpeed(Speed_Bps))
    return false;

  Receiver.HasDataInterrupt.Off();

  Transmitter.IsReadyInterrupt.Off();
  Transmitter.FrameSentInterrupt.Off();

  Receiver.On();
  Transmitter.On();

  return true;
}

/*
  Receive byte
*/
TBool me_Uart::GetByte(
  TUint_1 * Value
)
{
  me_Uart_Freetown::TReceiver Receiver;

  /*
    There is hardware magic at accessing transceiver buffer.
    Reading or writing it updates flags. So flags must be read first.
  */

  if (!Receiver.HasData())
    return false;

  TBool DataHasErorrs = Receiver.AreErrors();

  *Value = Receiver.Get();

  if (DataHasErorrs)
    return false;

  return true;
}

/*
  Send byte
*/
void me_Uart::SendByte(
  TUint_1 Value
)
{
  me_Uart_Freetown::TTransmitter Transmitter;

  while (!Transmitter.IsReady());

  Transmitter.Put(Value);

  while (!Transmitter.IsSent());
  Transmitter.ClearSentFlag();
}

/*
  Await byte

  Maybe forever.
*/
void me_Uart::WaitByte(
  TUint_1 * Value
)
{
  while (!GetByte(Value));
}

/*
  2024 # # # # #
  2025-07-13
  2025-08-25
  2025-08-26
  2025-08-29
*/
