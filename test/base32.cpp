
#include <string>
#include <vector>
#include <iterator>

#include <doctest/doctest.h>

#include <clockwork-base32.hpp>

#include "doctest-rapidcheck.hpp"

TEST_CASE ("test encoder with test vectors") {
    SUBCASE ("(empty)") {
        const std::vector<uint8_t> empty_input;
        std::vector<uint8_t>       actual;
        ClockworkBase32::encode (empty_input.begin (), empty_input.end (), std::back_inserter (actual));
        REQUIRE (actual.empty ());
    }
    SUBCASE ("\\xFF\\xFF") {
        const std::vector<uint8_t> input { 0xFFu, 0xFFu };
        std::vector<uint8_t>       actual;
        ClockworkBase32::encode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (actual.size (), 4);
        REQUIRE_EQ (actual[0], 'Z');
        REQUIRE_EQ (actual[1], 'Z');
        REQUIRE_EQ (actual[2], 'Z');
        REQUIRE_EQ (actual[3], 'G');
    }
    SUBCASE ("foobar") {
        const std::string input { "foobar" };
        const std::string expected { "CSQPYRK1E8" };
        std::string       actual;
        ClockworkBase32::encode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("Hello, world!") {
        const std::string input { "Hello, world!" };
        const std::string expected { "91JPRV3F5GG7EVVJDHJ22" };
        std::string       actual;
        ClockworkBase32::encode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("The quick brown fox jumps over the lazy dog.") {
        const std::string input { "The quick brown fox jumps over the lazy dog." };
        const std::string expected { "AHM6A83HENMP6TS0C9S6YXVE41K6YY10D9TPTW3K41QQCSBJ41T6GS90DHGQMY90CHQPEBG" };
        std::string       actual;
        ClockworkBase32::encode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
}

TEST_CASE ("test decoder with test vectors") {
    SUBCASE ("(empty)") {
        const std::vector<uint8_t> empty_input;
        std::vector<uint8_t>       actual;
        auto result = ClockworkBase32::decode (empty_input.begin (), empty_input.end (), std::back_inserter (actual));
        REQUIRE (result.has_value());
        REQUIRE (actual.empty ());
    }
    SUBCASE ("\\xFF\\xFF") {
        const std::string input { "ZZZG" };
        std::vector<uint8_t>       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE (result.has_value());
        REQUIRE_EQ (actual.size (), 2);
        REQUIRE_EQ (actual[0], 0xFFu);
        REQUIRE_EQ (actual[1], 0xFFu);
    }
    SUBCASE ("foobar") {
        const std::string input { "CSQPYRK1E8" };
        const std::string expected { "foobar" };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE (result.has_value());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("Hello, world!") {
        const std::string input { "91JPRV3F5GG7EVVJDHJ22" };
        const std::string expected { "Hello, world!" };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE (result.has_value());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("The quick brown fox jumps over the lazy dog.") {
        const std::string input { "AHM6A83HENMP6TS0C9S6YXVE41K6YY10D9TPTW3K41QQCSBJ41T6GS90DHGQMY90CHQPEBG" };
        const std::string expected { "The quick brown fox jumps over the lazy dog." };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE (result.has_value());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
}

namespace {
    std::string mutate (std::string src) {
        size_t cnt = 0;
        std::transform(src.begin (), src.end (), src.begin (), [&cnt](char x) -> char {
            auto c = cnt++;
            if (std::isupper(x) && (c % 2 == 0)) {
                return static_cast<char> (std::tolower(x));
            }
            if (x == '0') {
                switch (c % 3) {
                case 0:
                    return '0';
                case 1:
                    return 'o';
                case 2:
                    return 'O';
                }
            }
            if (x == '1') {
                switch (c % 5) {
                case 0:
                    return '1';
                case 1:
                    return 'i';
                case 2:
                    return 'I';
                case 3:
                    return 'l';
                case 4:
                    return 'L';
                }
            }
            return x;
        });
        return src;
    }
}

TEST_CASE ("property") {
    rc::check ("round trip test", [](const std::vector<uint8_t> &v) {
        std::string encoded;
        ClockworkBase32::encode (v.begin (), v.end (), std::back_inserter (encoded));
        // std::cerr << "encoded: " << encoded << std::endl;
        std::vector<uint8_t> decoded;
        auto                 result = ClockworkBase32::decode (encoded.begin ()
                                                               , encoded.end ()
                                                               , std::back_inserter (decoded));
        REQUIRE (result.has_value ());
        REQUIRE_EQ (decoded, v);
    });
    rc::check ("round trip with mutation", [](const std::vector<uint8_t> &v) {
        std::string encoded;
        ClockworkBase32::encode (v.begin (), v.end (), std::back_inserter (encoded));
        auto mutated = mutate(encoded);
        // std::cerr << "mutated: " << mutated << std::endl;
        std::vector<uint8_t> decoded;
        auto                 result = ClockworkBase32::decode (mutated.begin ()
                                                               , mutated.end ()
                                                               , std::back_inserter (decoded));
        REQUIRE (result.has_value ());
        REQUIRE_EQ (decoded, v);
    });
}
