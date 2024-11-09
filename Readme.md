# What

(2024-10/2024-11)

UART transceiver. Arduino library. 2 Mbps.

Another dive to low-level programming.

## Example interaction

Demo on 2 Mbps.

That's a bare-bones [example][Example] to test memory footprint and
code size. (For 2024-11-09 required memory is `9` bytes and
code size is `542` bytes.)

Sketch outputs `>` and start and then echoes everything except `^`.
On `^` it exits and outputs `<`.

```
>>kjfdkjlfdghfkhjlsdhdfskljhsldkjkjldfsjhkdsfkjlghdfslkhdflku34hi534293487r
kjfdkjlfdghfkhjlsdhdfskljhsldkjkjldfsjhkdsfkjlghdfslkhdflku34hi534293487r
^
<
```

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
* Interface
  * [Frontend][Interface_front]
  * [Backend][Interface_back]
* Implementation
  * [Frontend][Implementation_front]
  * [Backend][Implementation_back] (implementation is separated by modules)

## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Example]: examples/me_Uart/me_Uart.ino
[Interface_front]: src/me_Uart.h
[Interface_back]: src/me_Uart_Freetown.h
[Implementation_front]: src/me_Uart.cpp
[Implementation_back]: src/

[me_Console]: https://github.com/martin-eden/Embedded-me_Console
[me_InstallStandardStreams]: https://github.com/martin-eden/Embedded-me_InstallStandardStreams
[HardwareSerial]: https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
