/*
 * Filename: QuickCheck.ino
 *
 *     Note: The purpose of this sketch is to manually verify that
 *           every method and functionality of the class works as
 *           expected.
 *           It might be written differently but this is just a quick
 *           and dirty sketch, not really one to use to demonstrate
 *           the class capabilities: this is left to other better
 *           written sketches.
 */

#include <Arduino.h>
#include <MACAddress.h>

void common_use_cases() {
	// The default ctor provides a default MACAddress
	MACAddress mac0;
	Serial.print(F("\t(no args)  : "));
	Serial.println(mac0); // printTo method check

	// Single bytes (where byte == uint8_t) passed to the ctor
	MACAddress mac1 {0xFE, 0xED, 0xFE, 0xED, 0xFE, 0xED};
	Serial.print(F("\t(...)      : "));
	Serial.println(mac1);

	// Array of bytes (uint8_t) passed to the ctor
	byte arr[6] {0x01, 0xC0, 0xFF, 0xEE, 0x4D, 0xAD};
	MACAddress mac2 {arr};
	Serial.print(F("\t(arr)      : "));
	Serial.println(mac2);

	// Array of chars passed to the ctor
	MACAddress mac3 {"AB:CD:EF:01:23:45"};
	Serial.print(F("\t(char[])   : "));
	Serial.println(mac3);

	// __FlashStringHelper object passed to the ctor
	MACAddress mac4 {F("01:23:45:67:89:AB")};
	Serial.print(F("\t(F(char[])): "));
	Serial.println(mac4);

	// String object passed to the ctor
	String value {"A0:B0:C0:D0:E0:F0"};
	MACAddress mac5 {value};
	Serial.print(F("\t(String))  : "));
	Serial.println(mac5);

	Serial.println();
}

void mixed_case_use_cases() {
	// Array of chars with different cases (upper & lower cases) passed to the
	// ctor
	MACAddress mac0 {"aB:Cd:Ef:bC:AA:ff"};
	Serial.print(F("\t(char[])   : "));
	Serial.println(mac0);

	// Array of chars with different cases (upper & lower cases) passed to the
	// ctor
	MACAddress mac1 {"De:ad:Ca:fe:ba:be"};
	Serial.print(F("\t(char[])   : "));
	Serial.println(mac1);

	Serial.println();
}

void different_lengths_use_cases() {
	char value7[] = "10:20:30:40:50:60";
	MACAddress mac7 {value7};
	Serial.print('\t');
	Serial.print(value7);
	Serial.print(' ');
	Serial.println(mac7);

	char value16[] = "FF:FF:FF:FF:FF:FF";
	MACAddress mac16 {value16};
	Serial.print('\t');
	Serial.print(value16);
	Serial.print(' ');
	Serial.println(mac16);
	Serial.println();

	String value2 = "1:2:3:4:5:6";
	MACAddress mac2 {value2};
	Serial.print('\t');
	String value11 {"a:B:c:D:e:F"};
	MACAddress mac11 {value11};
	Serial.print('\t');
	Serial.print(value11);
	Serial.print(F("    => "));
	Serial.println(mac11);

	char *value12 = "F:E:D:c:b:a";
	MACAddress mac12 {value12};
	Serial.print('\t');
	Serial.print(value12);
	Serial.print(F("    => "));
	Serial.println(mac12);

	char value4[] = "0:0:0:0:0:0";
	MACAddress mac4 {value4};
	Serial.print('\t');
	Serial.print(value4);
	Serial.print(F("    => "));
	Serial.println(mac4);

	String value0 = ":::::";
	MACAddress mac0 {value0};
	Serial.print('\t');
	Serial.print(value0);
	Serial.print(F("          => "));
	Serial.println(mac0);

	char value1[] = ":::::1";
	MACAddress mac1 {value1};
	Serial.print('\t');
	Serial.print(value1);
	Serial.print(F("         => "));
	Serial.println(mac1);

	char value5[] {"::::1:"};
	MACAddress mac5 {value5};
	Serial.print('\t');
	Serial.print(value5);
	Serial.print(F("         => "));
	Serial.println(mac5);

	char value6[] {":::1::"};
	MACAddress mac6 {value6};
	Serial.print('\t');
	Serial.print(value6);
	Serial.print(F("         => "));
	Serial.println(mac6);

	char value8[] = "::1:::";
	MACAddress mac8 {value8};
	Serial.print('\t');
	Serial.print(value8);
	Serial.print(F("         => "));
	Serial.println(mac8);

	char value9[] = ":1::::";
	MACAddress mac9 {value9};
	Serial.print('\t');
	Serial.print(value9);
	Serial.print(F("         => "));
	Serial.println(mac9);

	char value10[] = "1:::::";
	MACAddress mac10 {value10};
	Serial.print('\t');
	Serial.print(value10);
	Serial.print(F("         => "));
	Serial.println(mac10);

	char value3[] = ":2:::5:";
	MACAddress mac3 {value3};
	Serial.print('\t');
	Serial.print(value3);
	Serial.print(F("        => "));
	Serial.println(mac3);

	char value13[] = "8::11:22:33:";
	MACAddress mac13 {value13};
	Serial.print('\t');
	Serial.print(value13);
	Serial.print(F("   => "));
	Serial.print(mac13);
	Serial.print(F("   "));
	for (auto i = 0; i != 6; ++i) {
		Serial.print(mac13[i], HEX);
		if (5 == i)
			Serial.println();
		else
			Serial.print(F(", "));
	}

	char value14[] = "8::CC:AA:BB:";
	MACAddress mac14 {value14};
	Serial.print('\t');
	Serial.print(value14);
	Serial.print(F("   => "));
	Serial.print(mac14);
	Serial.print(F("   "));
	for (auto i = 0; i != 6; ++i) {
		Serial.print(mac14[i], HEX);
		if (5 == i)
			Serial.println();
		else
			Serial.print(F(", "));
	}

	char value15[] = "8::C:A:B0:";
	MACAddress mac15 {value15};
	Serial.print('\t');
	Serial.print(value15);
	Serial.print(F("     => "));
	Serial.print(mac15);
	Serial.print(F("   "));
	for (auto i = 0; i != 6; ++i) {
		Serial.print(mac15[i], HEX);
		if (5 == i)
			Serial.println();
		else
			Serial.print(F(", "));
	}
}

void constructorsCheck() {
	Serial.println();
	Serial.println(F("Constructors Check"));
	Serial.println();

	common_use_cases();
	mixed_case_use_cases();
	different_lengths_use_cases();
}

void streamOutputCheck() {
	Serial.println();
	Serial.println(F("Stream Output Check"));
	Serial.println();

	MACAddress mac;
	Serial.print('\t');
	Serial.println(mac);

	mac.fromString("2:3:f:e:d:5");
	Serial.print('\t');
	Serial.println(mac);
}

void errorsCheck() {
	Serial.println();
	Serial.println(F("Errors Check"));
	Serial.println();

	MACAddress mac("1");
	Serial.print('\t');
	Serial.println(mac);

	bool rc = mac.fromString("a");
	Serial.print('\t');
	Serial.print(mac);
	Serial.print("  ");
	Serial.println(rc ? "true" : "false");

	MACAddress mac1;
	rc = mac1.fromString(F("FF:FF:FF:FF:FF:FF"));
	Serial.print('\t');
	Serial.print(mac1);
	Serial.print("  ");
	Serial.println(rc ? "true" : "false");
}

void setup() {
	Serial.begin(115200);
	delay(20);

	constructorsCheck();
	streamOutputCheck();
	errorsCheck();
}

void loop() {
}
