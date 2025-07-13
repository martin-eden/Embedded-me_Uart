# What

(2024-10/2024-11, 2025-07)

UART transceiver. Arduino library. 2 Mbps.

Another dive to low-level programming.

## Example interaction

Demo on 2 Mbps.

That's a bare-bones [example][SpeedDemo] to test memory footprint and
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


## Install/remove

Easy way is to clone [GetLibs][GetLibs] repo and run it's code.


## Compile

Zero-warnings compilation:

```bash
arduino-cli compile --fqbn arduino:avr:uno --quiet --warnings all . --build-property compiler.cpp.extra_flags="-std=c++1z"
```

## Code

* Interface
  * [Back][Interface_back]
  * [Mid][Interface_mid]
  * [Front][Interface_front]
* Implementation
  * [Back][Implementation_back]
  * [Mid][Implementation_mid] (separated by modules)
  * [Front][Implementation_front]
* [Examples][Examples]

## See also

* [My other embedded C++ libraries][Embedded]
* [My other repositories][Repos]

[Interface_back]: src/Bare/Interface.h
[Interface_mid]: src/Freetown/Interface.h
[Interface_front]: src/me_Uart.h
[Implementation_back]: src/Bare/Mapping.cpp
[Implementation_mid]: src/Freetown/
[Implementation_front]: src/me_Uart.cpp
[Examples]: examples/
[SpeedDemo]: examples/me_Uart/me_Uart.ino

[me_Console]: https://github.com/martin-eden/Embedded-me_Console
[me_InstallStandardStreams]: https://github.com/martin-eden/Embedded-me_InstallStandardStreams
[HardwareSerial]: https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.cpp

[GetLibs]: https://github.com/martin-eden/Embedded-Framework-GetLibs

[Embedded]: https://github.com/martin-eden/Embedded_Crafts/tree/master/Parts
[Repos]: https://github.com/martin-eden/contents
