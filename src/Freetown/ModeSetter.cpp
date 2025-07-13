// UART sync/async mode setter

/*
  Author: Martin Eden
  Last mod.: 2025-07-13
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

#include "Freetown/Interface.h"

#include "Bare/Interface.h"

using namespace me_Uart_Freetown;

using me_Uart_Bare::Uart;

/*
  Set asynchronous UART mode
*/
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
  2025-07-13
*/
