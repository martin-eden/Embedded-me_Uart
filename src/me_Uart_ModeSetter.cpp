// UART sync/async mode setter

/*
  Author: Martin Eden
  Last mod.: 2024-12-19
*/

/*
           Mode setter
   ------------------------
   |  Async  |            |
   |    |    |            |
   |  -[x]-  |            |
   |    |    |------------|
   |   [ ]   |  Polarity  |
   |         |    [ ]     |
   |       --|     |      |
   |         |   -[x]-    |
   |         |     |      |
   ------------------------
*/

#include <me_Uart.h>

#include <me_Uart_Freetown.h>

using namespace me_Uart::Freetown;

// Set asynchronous UART mode
void TModeSetter::SetAsyncMode()
{
  Uart->TransceiverMode = 0;
  ClearPolarityBit();
}

/*
  Clear polarity bit

  Polarity is synchronous mode stuff.

  For async mode this bit should be set to zero.
*/
void TModeSetter::ClearPolarityBit()
{
  Uart->Sync_Polarity = 0;
}

/*
  2024-11 #
*/
