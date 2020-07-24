
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
        REQUIRE_EQ (result, empty_input.end ());
        REQUIRE (actual.empty ());
    }
    SUBCASE ("\\xFF\\xFF") {
        const std::string input { "ZZZG" };
        std::vector<uint8_t>       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (result, input.end ());
        REQUIRE_EQ (actual.size (), 2);
        REQUIRE_EQ (actual[0], 0xFFu);
        REQUIRE_EQ (actual[1], 0xFFu);
    }
    SUBCASE ("foobar") {
        const std::string input { "CSQPYRK1E8" };
        const std::string expected { "foobar" };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (result, input.end ());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("Hello, world!") {
        const std::string input { "91JPRV3F5GG7EVVJDHJ22" };
        const std::string expected { "Hello, world!" };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (result, input.end ());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
    SUBCASE ("The quick brown fox jumps over the lazy dog.") {
        const std::string input { "AHM6A83HENMP6TS0C9S6YXVE41K6YY10D9TPTW3K41QQCSBJ41T6GS90DHGQMY90CHQPEBG" };
        const std::string expected { "The quick brown fox jumps over the lazy dog." };
        std::string       actual;
        auto result = ClockworkBase32::decode (input.begin (), input.end (), std::back_inserter (actual));
        REQUIRE_EQ (result, input.end ());
        REQUIRE_EQ (actual.size (), expected.size ());
        REQUIRE_EQ (actual, expected);
    }
}

namespace {
    std::string mutate (std::string src) {
        size_t cnt_lower = 0;
        size_t cnt_0 = 0;
        size_t cnt_1 = 0;
        std::transform(src.begin (), src.end (), src.begin (), [&cnt_lower, &cnt_0, &cnt_1](char x) -> char {
            if (std::isupper(x)) {
                ++cnt_lower;
                if (cnt_lower % 2 == 0) {
                    return static_cast<char> (std::tolower (x));
                }
            }
            if (x == '0') {
                ++cnt_0;
                switch (cnt_0 % 3) {
                case 0:
                    return '0';
                case 1:
                    return 'o';
                case 2:
                    return 'O';
                }
            }
            if (x == '1') {
                ++cnt_1;
                switch (cnt_1 % 5) {
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
    rc::prop ("roundtrip test", [] (const std::vector<uint8_t> &v) {
        std::string encoded;
        ClockworkBase32::encode (v.begin (), v.end (), std::back_inserter (encoded));
        // std::cerr << "encoded: " << encoded << std::endl;
        std::vector<uint8_t> decoded;
        auto                 result = ClockworkBase32::decode (encoded.begin ()
                                                               , encoded.end ()
                                                               , std::back_inserter (decoded));
        RC_ASSERT (result == encoded.end ());
        RC_ASSERT (decoded == v);
    });
    rc::prop ("roundtrip with mutation", [] (const std::vector<uint8_t> &v) {
        std::string encoded;
        ClockworkBase32::encode (v.begin (), v.end (), std::back_inserter (encoded));
        auto                 mutated = mutate (encoded);
        // std::cerr << "mutated: " << mutated << std::endl;
        std::vector<uint8_t> decoded;
        auto                 result  = ClockworkBase32::decode (mutated.begin ()
                                                                , mutated.end ()
                                                                , std::back_inserter (decoded));
        RC_ASSERT (result == mutated.end ());
        RC_ASSERT (decoded == v);
    });
    rc::prop ("roundtrip with incremental update", [] (const std::vector<uint8_t> &v1, const std::vector<uint8_t> &v2) {
        std::string              encoded;
        {
            auto                     out = std::back_inserter (encoded);
            ClockworkBase32::Encoder e;
            e (v1.begin (), v1.end (), out);
            e (v2.begin (), v2.end (), out);
            e.finalize (out);
        }
        auto                     split = *rc::gen::inRange<size_t> (0, encoded.size ()).as ("split");
        std::vector<uint8_t>     decoded;
        ClockworkBase32::Decoder dec;

        auto out = std::back_inserter (decoded);
        auto r1  = dec (encoded.begin (), encoded.begin () + split, out);
        RC_ASSERT (r1 == encoded.begin () + split);
        auto r2 = dec (encoded.begin () + split, encoded.end (), out);
        RC_ASSERT (r2 == encoded.end ());
        RC_ASSERT (std::equal (decoded.begin (), decoded.begin () + v1.size (), v1.begin ()));
        RC_ASSERT (std::equal (decoded.begin () + v1.size (), decoded.end (), v2.begin ()));
    });
    rc::prop ("decode error", [] (const std::vector<uint8_t> &v) {
        std::string encoded;
        ClockworkBase32::encode (v.begin (), v.end (), std::back_inserter (encoded));
        auto pos = *rc::gen::inRange<size_t> (0, v.size ()).as ("pos");
        encoded[pos]                = *rc::gen::element<char> ('*', '~', '$', '=', 'U').as ("patch");
        std::vector<uint8_t> decoded;
        auto                 result = ClockworkBase32::decode (encoded.begin (), encoded.end (), std::back_inserter (decoded));
        RC_ASSERT_FALSE(result == encoded.end ());
        RC_ASSERT (result == encoded.begin () + pos);
    });
}
