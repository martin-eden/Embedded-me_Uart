// UART sync/async mode setter

/*
  Author: Martin Eden
  Last mod.: 2025-10-22
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
  Uart->Sync_Polarity = 0;
}

/*
  2024-11 #
  2025-07-13
  2025-10-22
*/
