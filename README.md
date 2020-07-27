# Clockwork Base32 for C++

Header only implementation of the Clockwork Base32 encoder/decoder for C++.

Clockwork Base32 is a simple variant of Base32 inspired by Crockford's Base32.
See [Clockwork Base32 Specification](https://gist.github.com/szktty/228f85794e4187882a77734c89c384a8) for details.

## Example

```cpp
#include <clockwork-base32.hpp>

const std::string input { "Hello, world!" };
std::vector<uint8_t> encoded;
ClockworkBase32::encode (input.begin (), input.end (),
                         std::back_inserter (encoded));

std::string decoded;
auto result = ClockworkBase32::decode (encoded.begin (), encoded.end (),
                                       std::back_inserter (decoded));

assert (result == encoded.end ());
```
