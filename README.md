# CAN-Monitor-3000
A universal CAN bus monitoring program

![Screenshot](/doc/images/screenshot.png?raw=true)

## Summary
A gui-program for Windows and Linux which is ideally suited for looking at the data on unknown CAN buses and for making sense of it and decoding it.

The aim is to support as many different CAN-adapters as possible and to make the program easy to use and yet powerfull.

## Supported CAN-adapters
As of now, the following adapters are supported:

* SLCAN (used in a lot of hobby project CAN adapters, but also in some commercial ones)
  * Lawicel CAN232
  * Lawicel CANUSB
  * USBtin (by Thomas Fischl)
  * probably lots of others
* China - these can be found on ebay when searching the term "USB CAN" for about 18 dollars.
Sadly, there is no manufacturer name or device name given. They say "USB CAN Analyzer V7.1"
or something similar on the case. These mostly work, but the sadly the adapters themself are
a little buggy, and loose messages once in a while. Don't use if this is a problem for you.
![CAN adapter China](/doc/images/can-adapter-china.png?raw=true)
* Tritium Ethernet-CAN-bridge - these are quite nice but pretty obscure. They are sold by
the Australian company Tritium to be used with their solar car products. We use these regularly
at the Bochum Solar Car team.
* PCAN - supports different adapters from the German company PEAK System. These adapters are
very nice, but also expensive. The support in CAN Monitor 3000 is still work in progress. As
of now it is only tested with Windows and PCAN-USB. To use this adapter you need to install
it's Windows-driver and PCAN-Basic from PEAK Sytem's website.

## Format strings
You can type format strings into the field "Format String" to decode the data of the CAN message.

The format string consists of up to 8 fields seperated by a comma (",") that have the following format:

TYPE [MULTIPLIER][:[TEXT][NUM_DIGITS.NUM_DIGITS][TEXT]]

#### Type specifiers

|specifier|description|
|--|--|
|u8 or U8|unsigned 8 bits|
|u16|unsigned 16 bits little endian|
|u32|unsigned 32 bits little endian|
|s8 or S8|signed 8 bits|
|s16|signed 16 bits little endian|
|s32|signed 32 bits little endian|
|U16|unsigned 16 bits big endian|
|U32|unsigned 32 bits big endian|
|S16|signed 16 bits big endian|
|S32|signed 32 bits big endian|
|f|floating point 32 bits little endian (IEE754 single precision)|

#### Simple example
Consider a CAN message with the data bytes 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 decoded by the format string "u8,u8,u16,U32".

The first two bytes will be decoded by the two "u8" type specifiers and give "1,2".

The next two bytes will be decoded by the "u16" type specifier and give 3*1 + 4*256 = 1027 (little edian)

The next(last) 4 bytes will be decoded by the "U32" type specifier and give 5*1677216 + 6*65536 + 7*256 + 8 = 84281096

So the complete decoded message will be "1,2,1027,84281096"




