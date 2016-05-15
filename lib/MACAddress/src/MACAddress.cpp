/*
 *      @file: MACAddress.cpp
 *    @author: Alberto Vassena (aka sbaginov, Sigmund Baginov)
 *
 * Created on: 2016.03.26 16:19:38
 */

#include "MACAddress.h"

const char MACAddress::_hex[]{'0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void MACAddress::_clearAddress(void) {
  for (auto i = 0; i != _length; ++i)
    _address[i] = 255; // 255 == 0xFF
#ifdef DEBUG
  _showOctetAssignmentFailure();
#endif
}

bool MACAddress::_validOctets(int first, int second, int third, int fourth,
                              int fifth, int sixth) {
  return (_validOctet(first) && _validOctet(second) && _validOctet(third) &&
          _validOctet(fourth) && _validOctet(fifth) && _validOctet(sixth));
}

#ifdef DEBUG
void MACAddress::_showOctetAssignmentFailure(void) {
  Serial.println(F("Octet assignment failure: the address has been cleared."));
}
#endif

char MACAddress::_hexToNibble(char c) {
  static const uint8_t delta = _hex[10] - 10;
  c = toupper(c);
  return c - (isdigit(c) ? _hex[0] : delta);
}

MACAddress::MACAddress(void)
    : _address{static_cast<uint8_t>(0xDE), static_cast<uint8_t>(0xAD),
               static_cast<uint8_t>(0xBE), static_cast<uint8_t>(0xEF),
               static_cast<uint8_t>(0xFE), static_cast<uint8_t>(0xED)} {}

MACAddress::MACAddress(uint8_t first, uint8_t second, uint8_t third,
                       uint8_t fourth, uint8_t fifth, uint8_t sixth)
    : _address{first, second, third, fourth, fifth, sixth} {}

MACAddress::MACAddress(int first, int second, int third, int fourth, int fifth,
                       int sixth)
    : _address{static_cast<uint8_t>(first), static_cast<uint8_t>(second),
               static_cast<uint8_t>(third), static_cast<uint8_t>(fourth),
               static_cast<uint8_t>(fifth), static_cast<uint8_t>(sixth)} {
  // For consistency we check if any significant bit has been lost
  // by the static casts
  if (!_validOctets(first, second, third, fourth, fifth, sixth))
    _clearAddress();
}

MACAddress::MACAddress(uint8_t address[]) {
  memcpy(_address, address, _address_size);
}

MACAddress::MACAddress(int address[]) {
  bool out_of_bounds{false};

  for (uint8_t i = 0; i != _length; ++i) {
    int n = address[i];
    if (_validOctet(n))
      _address[i] = static_cast<uint8_t>(n);
    else {
      out_of_bounds = true;
      break;
    }
  }

  if (out_of_bounds)
    _clearAddress();
}

MACAddress::MACAddress(char address[]) { fromString(address); }

MACAddress::MACAddress(const char address[]) { fromString(address); }

MACAddress::MACAddress(String address) { fromString(address); }

MACAddress::MACAddress(const __FlashStringHelper *address) {
  fromString(address);
}

MACAddress::MACAddress(MACAddress &mac) {
  // self-assignment check
  if (this != &mac)
    memcpy(_address, mac._address, _address_size);
}

bool MACAddress::_fromString(char addr[]) {
  /*
   * Accepted MACs:
   *
   *   AB:CD:EF:01:23:45
   *   A:A:A:A:A:B
   *   1:C0:FF:EE:4D:AD
   *   40::CO:FF:EE:51
   *   40::C:0F:FE:E5
   *   08:00:12:34:56:78
   *   8::12:34:56:78
   *   8:0:12:34:56:78
   *   :::::1
   *   :::::
   */
  bool rc = true;
  uint8_t i = 0;
  uint8_t colons = 0;

  uint8_t len = strlen(addr);

  char *tmp = new char[len + 1];
  strncpy(tmp, addr, len);
  tmp[len] = 0;

  if (NULL != strchr(tmp, dash)) {
    if (5 != _replace_all(tmp, dash, colon)) {
      // Since this is not an EUI-48 MAC,
      // we restore tmp to its original and initial value
      // and - even though we know it is wrong - we just
      // let the rest of this method discover it and
      // act accordingly.
      strncpy(tmp, addr, len);
      tmp[len] = 0;
    }
  }

  char *p = tmp; // Serial.printf(" BEG> addr: %p, p: %p\n", addr, p);

  bool first = true;
  uint8_t x = 0;

  uint8_t n = 0, length = strlen(tmp);

  while (*p) {
    char c = *p++;
    ++n;

    if (isxdigit(c)) {
      c = _hexToNibble(c);
      x = first ? c : ((x << 4) + c);
      first = !first;

    } else if (c == colon) {
      if (colons == 5) {
        // Too many colons (there must be 3 colons)
        rc = false;
        break;
      }

      _address[i] = x;
      ++i;
      x = 0;
      ++colons;
      first = true;
    } else {
      // Invalid char
      rc = false;
      break;
    }

    if (n == length) {
      // Last byte
      _address[i] = x;
      break;
    }
  }

  // Too few colons (5 expected)
  if (colons != 5)
    rc = false;

  if (!rc)
    _clearAddress();

  delete[] tmp;

  return rc;
}

bool MACAddress::fromString(char address[]) { return _fromString(address); }

bool MACAddress::fromString(const char address[]) {
  char addr[_str_size];
  strcpy(addr, (char *)(address));
  return _fromString(addr);
}

bool MACAddress::fromString(const __FlashStringHelper *address) {
  char addr[_str_size];
  strcpy_P(addr, (PGM_P)address);
  return _fromString(addr);
};

bool MACAddress::fromString(String address) {
  // _fromString() is not directly callable
  return fromString(address.c_str());
};

char *MACAddress::c_str(Representation representation) {
  char mask[] = "%02X:%02X:%02X:%02X:%02X:%02X"; // TODO: move to .code

  switch (representation) {
  case Representation::COMMON_FULL:
    snprintf(_strCommonFull, _str_size, mask, _address[0], _address[1],
             _address[2], _address[3], _address[4], _address[5]);
    return _strCommonFull;
    break;
  case Representation::EUI48:
    for (auto i = 4; i <= 24; i += 5)
      mask[i] = dash;
    snprintf(_strEUI48, _str_size, mask, _address[0], _address[1], _address[2],
             _address[3], _address[4], _address[5]);
    return _strEUI48;
    break;
  case Representation::COMMON_COMPACT:
    break;
  }

  uint8_t j{0}, colons{0};

  nibbles addr[_length];
  for (auto i = 0; i != _length; ++i)
    addr[i].n = _address[i];

  for (auto i = 0; i != _length; ++i) {
    if (addr[i].left && addr[i].right) {
      _strCommonCompact[j++] = _hex[addr[i].left];
      _strCommonCompact[j++] = _hex[addr[i].right];
    } else if (0 == addr[i].left) {
      if (addr[i].right)
        _strCommonCompact[j++] = _hex[addr[i].right];
      // else ; // do nothing, we'll just add (if possible) the colon
    } else if (!addr[i].right) {
      _strCommonCompact[j++] = _hex[addr[i].left];
      _strCommonCompact[j++] = _hex[0];
    }

    if (colons < 5) {
      _strCommonCompact[j++] = colon;
      ++colons;
    }
  }
  _strCommonCompact[j] = '\0';

  return _strCommonCompact;
}

uint8_t MACAddress::operator[](int i) const { return _address[i]; }

uint8_t &MACAddress::operator[](int i) { return _address[i]; }

MACAddress &MACAddress::operator=(const uint8_t *address) {
  memcpy(_address, address, sizeof(_address));
  return *this;
}

MACAddress &MACAddress::operator=(const MACAddress &mac) {
  // self-assignment check
  if (this != &mac)
    for (auto i = 0; i != _length; ++i)
      _address[i] = mac[i];

  return *this;
}

bool MACAddress::operator==(const MACAddress &mac) const {
  return 0 == memcmp(_address, mac._address, _address_size);
}

bool MACAddress::operator==(char *address) const {
  MACAddress mac{address};
  return operator==(mac);
}

bool MACAddress::operator==(const char *address) const {
  MACAddress mac{address};
  return operator==(mac);
}

bool MACAddress::operator==(const __FlashStringHelper *address) const {
  MACAddress mac{address};
  return operator==(mac);
}

bool MACAddress::operator==(String address) const {
  MACAddress mac{address};
  return operator==(mac);
}

bool MACAddress::operator==(const uint8_t *address) const {
  return 0 == memcmp(_address, address, _address_size);
}

bool MACAddress::operator!=(const MACAddress &mac) const {
  return !(*this == mac);
}

bool MACAddress::operator!=(char *address) const { return !(*this == address); }

bool MACAddress::operator!=(const char *address) const {
  return !(*this == address);
}

bool MACAddress::operator!=(const __FlashStringHelper *address) const {
  return !(*this == address);
}

bool MACAddress::operator!=(String address) const {
  return !(*this == address);
}

bool MACAddress::operator!=(const uint8_t *address) const {
  return !(*this == address);
}

MACAddress &MACAddress::_sum(int64_t n, bool add) {
  int64_t x{static_cast<int64_t>(0)};
  int64_t divisor{_max24 + 1}; // default divisor

  /*
     Pointers to functions to just have the initial switch and not the 2nd
     require extra code since the methods signatures are different:

       - uint32_t getExtensionId24() == (uint32_t *)func(void)
       - uint32_t getExtensionId28() == (uint32_t *)func(void)
       - uint64_t getExtensionId36() == (uint64_t *)func(void)

       - void setExtensionId24(uint32_t) == (void *)func(uint32_t)
       - void setExtensionId28(uint32_t) == (void *)func(uint32_t)
       - void setExtensionId36(uint64_t) == (void *)func(uint64_t)

     For this reason and because I don't want to be too smart, the 2nd switch
     is going to stay.
   */
  switch (ma) {
  case MA::L:
    // divisor = _max24 + 1; // default divisor
    x = getExtensionId24();
    break;

  case MA::M:
    divisor = _max28 + 1;
    x = getExtensionId28();
    break;

  case MA::S:
    divisor = _max36 + 1;
    x = getExtensionId36();
    break;
  }

  n %= divisor; // assure the range by using circular arithmetic
  add ? x += n : x -= n;

  // TODO: check the circular arithmetic when x < 0;

  x %= divisor; // assure the range by using circular arithmetic

  switch (ma) {
  case MA::L:
    setExtensionId24(x);
    break;

  case MA::M:
    setExtensionId28(x);
    break;

  case MA::S:
    setExtensionId36(x);
    break;
  }

  return *this;
}

MACAddress &MACAddress::operator+=(int64_t n) {
  return (n < 0) ? operator-=(-n) : _sum(n, true);
}

MACAddress &MACAddress::operator-=(int64_t n) {
  return (n < 0) ? operator+=(-n) : _sum(n, false);
}

MACAddress &MACAddress::operator+(int64_t n) { return MACAddress(*this) += n; }

MACAddress &MACAddress::operator-(int64_t n) { return MACAddress(*this) -= n; }

MACAddress &MACAddress::operator++(void) { return operator+=(1); }

MACAddress &MACAddress::operator--(void) { return operator-=(1); }

MACAddress::operator uint8_t *(void) { return _address; }

MACAddress::operator char *(void) { return c_str(); }

uint32_t MACAddress::getOUI24(void) {
  id32 rc;

  for (auto i = 0; i != 3; ++i)
    rc.octet[i] = _address[2 - i];

  return rc.n;
}

uint32_t MACAddress::getOUI20(void) {
  id32 rc;

  for (auto i = 0; i != 3; ++i)
    rc.octet[i] = _address[2 - i];

  rc.n >>= 4;

  return rc.n;
}

uint32_t MACAddress::getOUI12(void) {
  id32 rc;

  for (auto i = 0; i != 2; ++i)
    rc.octet[i] = _address[1 - i];

  rc.n >>= 4;

  return rc.n;
}

uint32_t MACAddress::getExtensionId24(void) {
  id32 rc;

  for (auto i = 0; i != 3; ++i)
    rc.octet[i] = _address[_length - 1 - i];

  return rc.n;
}

uint32_t MACAddress::getExtensionId28(void) {
  id32 rc;

  for (auto i = 0; i != 4; ++i)
    rc.octet[i] = _address[_length - 1 - i];

  // reread the last octet and adjust it
  rc.octet[3] = getRightNibble(rc.octet[3]);

  return rc.n;
}

uint64_t MACAddress::getExtensionId36(void) {
  id36 rc;

  for (auto i = 0; i != 5; ++i)
    rc.octet[i] = _address[_length - 1 - i];

  // reread the last octet and adjust it
  rc.octet[4] = getRightNibble(rc.octet[4]);

  return rc.n;
}

void MACAddress::setExtensionId24(uint32_t n) {
  id32 x;

  x.n = n;

  _address[3] = x.octet[2];
  _address[4] = x.octet[1];
  _address[5] = x.octet[0];
}

void MACAddress::setExtensionId28(uint32_t n) {
  id32 x;
  x.n = n;

  _address[2] = getLeftNibble(_address[2]) + getRightNibble(x.octet[3]);
  _address[3] = x.octet[2];
  _address[4] = x.octet[1];
  _address[5] = x.octet[0];
}

void MACAddress::setExtensionId36(uint64_t n) {
  id36 x;
  x.n = n;

  _address[1] = getLeftNibble(_address[1]) + getRightNibble(x.octet[4]);
  _address[2] = x.octet[3];
  _address[3] = x.octet[2];
  _address[4] = x.octet[1];
  _address[5] = x.octet[0];
}

uint8_t MACAddress::_replace_all(char *target, char what, char with) {
  uint8_t rc = 0;

  for (auto i = 0; i != static_cast<uint8_t>(strlen(target)); ++i)
    if (what == target[i]) {
      target[i] = with;
      ++rc;
    }

  return rc;
}

char *MACAddress::uint64ToHex(uint64_t n) {
  // method by ataraxic
  // @see
  // http://stackoverflow.com/questions/9225567/how-to-print-a-int64-t-type-in-c
  static char buffer[32]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                         0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char *p = &buffer[31];
  auto base = 16;

  do {
    *p = _hex[n % base];
    --p;
    n /= base;
  } while (n);

  return p + 1;
}

/*
    @parameter what: [0, 255].
    @parameter readyToBeJoined: true(default)|false.

    [code]
    bool rc;
    uint8_t what { 0xFA };

    if(readyToBeJoined) {
      rc = (0xF0 == getLeftNibble(what)) == (0xF0 == getLeftNibble(what,
                                                                   true));
    } else {
      rc = 0x0F == getLeftNibble(what, false);
    }
    // Here: true == rc
    [/code]
 */
uint8_t MACAddress::getLeftNibble(uint8_t what, bool readyToBeJoined) {
  uint8_t rc{static_cast<uint8_t>(what >> 4)};
  if (readyToBeJoined)
    rc <<= 4;
  return rc;
}

/*
    @parameter what: [0, 255].

    [code]
    uint8_t what { 0xFA };

    // Below: true == rc1
    bool rc1 = 0x0A == getRightNibble(what, false);

    // below: true == rc2
    bool rc2 = 0xFA == (getLeftNibble(what) + getRightNibble(what));
    [/code]
 */
uint8_t MACAddress::getRightNibble(uint8_t what) {
  uint8_t rc{static_cast<uint8_t>(what << 4)};
  return rc >>= 4;
}

size_t MACAddress::printTo(Print &p) const {
  size_t n{static_cast<size_t>(0)};

  for (auto i = 0; i < _length; i++) {
    auto value = _address[i];
    if (value < 0x10)
      n += p.print(_hex[0]);
    n += p.print(value, HEX);
    if (i != 5)
      n += p.print(colon);
  }

  return n;
}
