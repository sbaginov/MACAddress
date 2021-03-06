/*
 * This sketch shows how the
 */
#include <Arduino.h>
#include <MACAddress.h>

const uint32_t BAUD_RATE{115200};

void setup(void) {
  Serial.begin(BAUD_RATE);

  // initialized by passing 6 octets
  MACAddress mac{static_cast<uint8_t>(0xDA), static_cast<uint8_t>(0xD5),
                 static_cast<uint8_t>(0xC0), static_cast<uint8_t>(0xFF),
                 static_cast<uint8_t>(0xEE), static_cast<uint8_t>(0x0)};

  Serial.print(F("MAC Address: "));

  // the following line "secretly" calls the MACAddress::printTo() method
  Serial.println(mac);

  // initialized by a RAM stored ASCIIZ string
  MACAddress mac1{"DA:D5:C0:FF:EE:00"};

  // initialized by a FLASH stored ASCIIZ string
  MACAddress mac2{F("DA:D5:C0:FF:EE:00")};

  char *str3{"DA:D5:C0:FF:EE:00"}; // classic ASCIIZ string

  // classic array of characters with the specified array length
  char arr4[6]{static_cast<char>(0xDA), static_cast<char>(0xD5),
               static_cast<char>(0xC0), static_cast<char>(0xFF),
               static_cast<char>(0xEE), static_cast<char>(0x0)};

  // classic array of characters whose length is automatically calculated
  char arr5[]{static_cast<char>(0xDA), static_cast<char>(0xD5),
              static_cast<char>(0xC0), static_cast<char>(0xFF),
              static_cast<char>(0xEE), static_cast<char>(0x0)};

  // classic array of const chars whose length is automatically calculated
  const char arr6[]{static_cast<char>(0xDA), static_cast<char>(0xD5),
                    static_cast<char>(0xC0), static_cast<char>(0xFF),
                    static_cast<char>(0xEE), static_cast<char>(0x0)};

  // further initializations
  MACAddress mac3{str3}, mac4{arr4}, mac5{arr5}, mac6{arr6};

  // array of MACAddresses definition and initialization
  MACAddress macs[]{mac1, mac2, mac3, mac4, mac5, mac6};

  uint8_t n{0xA0};
  for (auto i = 0; i != 6; ++i) {
    Serial.print(i + 1);
    Serial.print(F(": "));
    Serial.print(macs[i].c_str(Representation::EUI48));
    Serial.print(F(" => "));
    macs[i] += n + i * 0x10;
    Serial.println(macs[i].c_str(Representation::EUI48));
  }
}

void loop(void) {}
