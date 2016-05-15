#ifndef _MACAddress_h_
#define _MACAddress_h_

/*
 *      @file: MACAddress.h
 *    @author: Alberto Vassena (aka sbaginov, Sigmund Baginov)
 *
 * Created on: 2016.03.26 16:19:38
 */

#include <Arduino.h>

enum class Representation { EUI48, COMMON_FULL, COMMON_COMPACT };

enum class MA {
  L, // 24 bits for the userAddress
  M, // 28  "    "   "      "
  S  // 36  "    "   "      "
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * The MACAddress class is and should be useful to work with many administered
 * PCs & IOT devices to generate subsequent and hopefully unique EUI-48
 * (otherwise known as MAC) addresses in the three different sets:
 *   - MA-L (OUI & Extension Id lengths: 24 bits)
 *   - MA-M (OUI length: 20 bits, Extension Id length: 28 bits)
 *   - MA-S (OUI length: 12 bits, Extension Id length: 36 bits)
 *
 * To the extension Id - in fact - a progressive number can be easily added and
 * subtracted using the proper methods.
 *
 * The default constructor provides a default address, the others accept single
 * octets, strings, arrays of unsigned short integers and other MAC addresses.
 *
 * Every address can be easily changed by simple + and - operations, by
 * accessing each single octet using the square brackets array notation, or by
 * simply passing a string to override it immediately.
 *
 * MAC addresses can even be tested for equality and inequality and quickly read
 * as sequences of characters ready to be printed in different formats.
 *
 * To quickly access the address as an array of characters or unsigned integers,
 * casting methods are provided.
 *
 * The ancestor
 *  - Printable
 * and the public method
 * - virtual size_t printTo(Print& p) const;
 * allow MACAddress objects to be printed to streams.
 */
class MACAddress : public Printable {
private:
  typedef union {
    uint32_t n{static_cast<uint32_t>(0)};
    uint8_t octet[4];
  } id32;
  typedef union {
    uint64_t n{static_cast<uint64_t>(0)};
    struct {
      uint8_t octet[5];
      uint8_t extra[3];
    };
  } id36;
  typedef union {
    uint8_t n{static_cast<uint8_t>(0)}; // initialization added for consistency
    struct {
      uint8_t right : 4;
      uint8_t left : 4;
    };
  } nibbles;

  uint8_t getLeftNibble(uint8_t what, bool readyToBeJoined = true);
  uint8_t getRightNibble(uint8_t what);

  // Constants shared by all MACAddress objects
  static const char colon = ':';
  static const char dash = '-';
  static const uint8_t _length{6};
  static const uint8_t _address_size{sizeof(uint8_t) * _length};
  static const uint8_t _str_size{17 + 1};
  static const char _hex[];
  static const uint32_t _max24{0xFFFFFFul};    // [0, 2^24 - 1] == [0, 2^24)
  static const uint32_t _max28{0xFFFFFFFul};   // [0, 2^28 - 1] == [0, 2^28)
  static const uint64_t _max36{0xFFFFFFFFFul}; // [0, 2^36 - 1] == [0, 2^36)

  uint8_t _address[_length];
  char _strCommonFull[_str_size];
  char _strCommonCompact[_str_size];
  char _strEUI48[_str_size];

protected:
  bool _fromString(char addr[]);
  char _hexToNibble(char c);
  void _clearAddress(void);
  bool _validOctet(int n) { return n >= 0 && n < 256; };
  bool _validOctets(int first, int second, int third, int fourth, int fifth,
                    int sixth);
#ifdef DEBUG
  void _showOctetAssignmentFailure(void);
#endif
  uint8_t _replace_all(char *target, char what, char with);
  MACAddress &_sum(int64_t n, bool add);

public:
  MA ma{MA::L};
  char *uint64ToHex(uint64_t n);
  uint32_t getOUI24(void);
  uint32_t getExtensionId24(void);
  void setExtensionId24(uint32_t n);

  uint32_t getOUI20(void);
  uint32_t getExtensionId28(void);
  void setExtensionId28(uint32_t n);

  uint32_t getOUI12(void);
  uint64_t getExtensionId36(void);
  void setExtensionId36(uint64_t n);

  MACAddress(void);
  MACAddress(uint8_t address[]);
  MACAddress(int address[]);
  MACAddress(char address[]);
  MACAddress(const char address[]);
  MACAddress(const __FlashStringHelper *address);
  MACAddress(String address);
  MACAddress(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth,
             uint8_t fifth, uint8_t sixth);
  MACAddress(int first, int second, int third, int fourth, int fifth,
             int sixth);
  MACAddress(MACAddress &mac);

  bool fromString(const char address[]);
  bool fromString(char address[]);
  bool fromString(const __FlashStringHelper *address);
  bool fromString(String address);
  char *c_str(Representation representation = Representation::COMMON_FULL);

  // Copy operator to allow initialization of MACAddress objects from different
  // types
  MACAddress &operator=(const uint8_t *address);
  MACAddress &operator=(const MACAddress &mac);

  // Overloaded index operator to allow getting and setting individual octets of
  // the address
  uint8_t operator[](int i) const;
  uint8_t &operator[](int i);

  // Overloaded comparison operator to allow checking for equality between
  // different MACAddress objects / arrays
  bool operator==(const MACAddress &mac) const;
  bool operator==(char *address) const;
  bool operator==(const char *address) const;
  bool operator==(const __FlashStringHelper *address) const;
  bool operator==(String address) const;
  bool operator==(const uint8_t *address) const;

  // Overloaded inequality operator to allow checking for inequality between
  // different MACAddress objects / arrays
  bool operator!=(const MACAddress &mac) const;
  bool operator!=(char *address) const;
  bool operator!=(const char *address) const;
  bool operator!=(const __FlashStringHelper *address) const;
  bool operator!=(String address) const;
  bool operator!=(const uint8_t *address) const;

  MACAddress &operator+=(int64_t n);
  MACAddress &operator-=(int64_t n);

  MACAddress &operator+(int64_t n);
  MACAddress &operator-(int64_t n);

  // pre-increment operator overloading (no post-increment)
  MACAddress &operator++(void);
  // pre-decrement operator overloading (no post-decrement)
  MACAddress &operator--(void);

  // Overloaded cast operator to allow MACAddress objects to be used
  // where (a pointer to) a six bytes uint8_t array is expected
  operator uint8_t *(void);

  // Overloaded cast operator to allow MACAddress objects to be used
  // where a pointer to a string is expected
  operator char *(void);

  virtual size_t printTo(Print &p) const;
};

#endif // _MACAddress_h_
