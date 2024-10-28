# What

(2024-10)

UART transceiver. Arduino library.

Another dive to low-level programming.

## Details

System output. Skillcheck. One of the things every hacker should write
for himself.

It does not use `HardwareSerial.h`. It does not use `avr/io.h`.
It directly accesses I/O registers. The way I want it and under
names I've chosen.

Internally I've built it without ISRs and without circular buffers.
No need for them in my projects (yet?).

I've switched [me_Console][me_Console] and
[me_InstallStandardStreams][me_InstallStandardStreams] to this.
Goodbye [HardwareSerial][HardwareSerial]!

## Example interaction

Some `for()` to verify output speed. Then echo everything until
`~` character.

```
[me_Uart] Start.
If you can see this it means [me_Uart] works
for transmission.

We'll print lot of lines to measure transfer time.
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
Transferred 01024 bytes in 0000000177 ms.
Echo until '~' character..
dffldg fg
dffldg fg
dsfg
dsfg
gsdgfgs  ~
gsdgfgs  ~[me_Uart] Done.
```

## Requirements

  * arduino-cli
  * bash


## Install

Run [Install.sh](Install.sh).


## Remove

Run [Remove.sh](Remove.sh).


## Compile

Zero-warnings compilation:

```bash
arduino-cli compile --fqbn arduino:avr:uno --quiet --warnings all . --build-property compiler.cpp.extra_flags="-std=c++1z"
```

## Code

* [Example][Example]
* [Interface][Interface]
* Implementation
  * [Frontend][Implementation_front]
  * [Backend][Implementation_back]

## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Example]: examples/me_Uart/me_Uart.ino
[Interface]: src/me_Uart.h
[Implementation_front]: src/me_Uart.cpp
[Implementation_back]: src/me_Uart_Freetown.cpp

[me_Console]: https://github.com/martin-eden/Embedded-me_Console
[me_InstallStandardStreams]: https://github.com/martin-eden/Embedded-me_InstallStandardStreams
[HardwareSerial]: https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
