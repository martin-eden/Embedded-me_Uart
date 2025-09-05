// Linking USART structure to address

/*
  Author: Martin Eden
  Last mod.: 2025-09-05
*/

#include "Bare/Interface.h"

/*
  Ancient language C compiles all files in directory.
  It lacks module concept. It just has paste macro #include.

  So when "header" contains assignment and that header
  is pasted into several .cpp files, there is "multiple definitions"
  error.

  So yeah, lets create one-line .cpp file to be sure assignment
  is done only once.
*/
me_Uart_Bare::TUart * me_Uart_Bare::Uart = (me_Uart_Bare::TUart *) 192;

/*
  2025-07-13
*/
