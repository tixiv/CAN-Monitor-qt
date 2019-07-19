# CAN-Monitor-3000
A universal CAN bus monitoring program

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
* Tritium Ethernet-CAN-bridge - these are quite nice but pretty obscure. They are sold by
the Australian company Tritium to be used with their solar car products. We use these regularly
at the Bochum Solar Car team.
* PCAN - supports different adapters from the German company PEAK System. These adapters are
very nice, but also expensive. The support in CAN Monitor 3000 is still work in progress. As
of now it is only tested with Windows and PCAN-USB. To use this adapter you need to install
it's Windows-driver and PCAN-Basic from PEAK Sytem's website.
