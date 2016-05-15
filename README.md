# MACAddress
Genuino/Arduino Library to generate and operate on and with MAC addresses.

The original purpose was to use it in a sort of massive firmware project on a MEGA or a Nano where a few customizations were to be automatically accomplished based on different static IP addresses.
```
#include <Arduino.h>
#include <Ethernet.h>
#include <IPAddress.h>

#include <MACAddress.h>

const uint32_t BAUD_RATE{115200};

void setup(void) {
  Serial.begin(BAUD_RATE);
  MACAddress mac{0xCA, 0xFE, 0xBA, 0xBE, 0, 0x01};

  Serial.print("Current MAC Address is ");
  if(0 == Ethernet.begin(mac)) {
    Serial.println(mac);
    Serial.println("The DHCP server has assigned an IP Address.")
  } else {
    mac += 0xA0;
    Serial.println(mac);

    IPAddress ip{192, 168, 1, 234};
    Ethernet.begin(mac, ip);
    Serial.print("A static IP Address (");
    Serial.print(ip);
    Serial.println(") configuration has been tried.");
  }
}

void loop(void) {}
```
The main class then evolved on its own and now supports _initialization from array of chars, ints, strings and single octets_.

A MACAddress object can be easily _casted to an array_ of ints, _printed to streams_, _converted to chars_ according to common usage and _EUI-48_ specifications, and even produce a _compact array of chars_ whenever possible and requested (in case of leading zeros or zeroed octets).

Single octets may be changed at any time by using the _array notation_; the _extension id_ may be easily requested and set according to the 24, 28 and 36 bits length and a few operations such as _comparisons_ for equality and inequality, _addition_ and _subtraction_ are supported.

The library may be larger than probably most of us would ever need, but in some cases it can be really helpful not to worry about conversions, memory allocation or whatever.

Any feedback on it and on its usage is and will always be greatly appreciated.
