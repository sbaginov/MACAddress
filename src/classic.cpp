#include <Arduino.h>
#include <MACAddress.h>

const uint32_t BAUD_RATE {115200};

void setup() {
	Serial.begin(BAUD_RATE);

	MACAddress mac {0xDA, 0xD5, 0xC0, 0xFF, 0xEE, 0x0};     // initialized by passing 6 octets

	Serial.print(F("MAC Address: "));
	Serial.println(mac);                                    // this line secretly calls the MACAddress::printTo() method

	MACAddress mac1 {"DA:D5:C0:FF:EE:00"};                  // initialized by a RAM   stored ASCIIZ string
	MACAddress mac2 {F("DA:D5:C0:FF:EE:00")};               // initialized by a FLASH stored ASCIIZ string

	char *str3  {"DA:D5:C0:FF:EE:00"};                      // classic ASCIIZ string
	char arr4[6] {0xDA, 0xD5, 0xC0, 0xFF, 0xEE, 0x0};       // classic array of characters with the specified array length
	char arr5[] {0xDA, 0xD5, 0xC0, 0xFF, 0xEE, 0x0};        // classic array of characters whose length is automatically calculated
	const char arr6[] {0xDA, 0xD5, 0xC0, 0xFF, 0xEE, 0x0};  // classic array of const chars whose length is automatically calculated

	MACAddress mac3 {str3}, mac4 {arr4}, mac5 {arr5}, mac6 {arr6};

	MACAddress macs[] {mac1, mac2, mac3, mac4, mac5, mac6};

	uint8_t n {0xA0};
	for(auto i = 0; i != 6; ++i) {
		Serial.print(i + 1);
		Serial.print(F(": "));
		Serial.print(macs[i].c_str(Representation::EUI48));
		Serial.print(F(" => "));
		macs[i] += n + i * 0x10;
		Serial.println(macs[i].c_str(Representation::EUI48));
	}
}

void loop() {
}
