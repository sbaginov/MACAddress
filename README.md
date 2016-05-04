# MACAddress
Genuino/Arduino Library to generate and operate on and with MAC addresses. It supports initialization from array of chars, strings and single octets. A MAC address can be easily printed to streams, converted to chars according to canonical usage, casted to an array of ints and even produce a compressed array of chars in case of leading zeros.
Single octets may be changed using the array notation, the extension id may be easily requested and set according to the 24, 28 and 36 bits length and a few operations such as comparisons for equality, addition and subtraction are supported.
The library may be larger than probably most of us would ever need, but in some cases it can be really helpful not to worry about conversions, memory allocation or whatever. The original purpose was to use it in a sort of massive firmware project on a MEGA or a Nano where a few customizations are to be automatically accomplished and the data to be manipulated has to adapt its shape to preexisting APIs that cannot be changed or just are easier to use if treated as a black box.
