// UART access. Frontend functions

/*
  Author: Martin Eden
  Last mod.: 2024-12-18
*/

#include "me_Uart.h"

#include <me_BaseTypes.h>
#include <me_Uart_Freetown.h>

using namespace me_Uart;

// Set-up for given speed (no parity, 8 data bits, 1 stop bit)
TBool me_Uart::Init(
  TUint_4 Speed_Bps
)
{
  Freetown::TModeSetter ModeSetter;
  Freetown::TFrameSetter FrameSetter;
  Freetown::TSpeedSetter SpeedSetter;
  Freetown::TTransmitter Transmitter;
  Freetown::TReceiver Receiver;

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

// Send byte
void me_Uart::SendByte(
  TUint_1 Value
)
{
  Freetown::TTransmitter Transmitter;

  // This "while" shouldn't take long
  while (!Transmitter.IsReady());

  Transmitter.Put(Value);
}

// Receive byte
TBool me_Uart::GetByte(
  TUint_1 * Value
)
{
  Freetown::TReceiver Receiver;

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

// Await byte. Maybe forever
void me_Uart::WaitByte(
  TUint_1 * Value
)
{
  while (!GetByte(Value));
}

// ( Wrappers for TOperation
TBool me_Uart::Op_GetByte(
  TAddress Data,
  TAddress Unused [[gnu::unused]]
)
{
  return GetByte((TUint_1 *) Data);
}

TBool me_Uart::Op_PutByte(
  TAddress Data,
  TAddress Unused [[gnu::unused]]
)
{
  SendByte(*(TUint_1 *) Data);

  return true;
}

// )

/*
  2024-10 # # #
  2024-11 #
  2024-12-18
*/
