//
// Copyright (c) 2020 Masashi Fujita all rights reserved.
//
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <tuple>

namespace ClockworkBase32 {

    namespace detail {
        // clang-format off
        inline const std::array<uint8_t, 32> symEncode {
            {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'J', 'K',
                'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'X',
                'Y', 'Z'
            }
        };

        inline const std::array<uint8_t, 256> symDecode {
            {
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0x00u, 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u,
                0x08u, 0x09u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x10u,
                0x11u, 0x01u, 0x12u, 0x13u, 0x01u, 0x14u, 0x15u, 0x00u,
                0x16u, 0x17u, 0x18u, 0x19u, 0x1Au, 0xFFu, 0x1Bu, 0x1Cu,
                0x1Du, 0x1Eu, 0x1Fu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0x0Au, 0x0Bu, 0x0Cu, 0x0Du, 0x0Eu, 0x0Fu, 0x10u,
                0x11u, 0x01u, 0x12u, 0x13u, 0x01u, 0x14u, 0x15u, 0x00u,
                0x16u, 0x17u, 0x18u, 0x19u, 0x1Au, 0xFFu, 0x1Bu, 0x1Cu,
                0x1Du, 0x1Eu, 0x1Fu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
                0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu
            }
        };
        // clang-format on

        static constexpr uint8_t mask (size_t n) {
            return static_cast<uint8_t> ((1u << n) - 1u);
        }

        static constexpr uint8_t encode (uint16_t x) {
            return detail::symEncode[x & 0x1Fu];
        }

        static constexpr std::optional<uint8_t> decode (uint8_t ch) {
            auto v = detail::symDecode[ch];
            if (v < 32) {
                return { v };
            }
            return {};
        }
    }

    class Encoder {
        uint8_t bits_      = 0;
        uint8_t cntBits_   = 0;
        uint8_t finalized_ = 0;
    public:
        Encoder () = default;

        [[nodiscard]] bool finalized () const { return finalized_ != 0; }

        template <typename OutIt_>
            void operator() (OutIt_ out, uint8_t v) {
                encode (out, v);
            }

        template <typename OutIt_>
            void encode (OutIt_ out, uint8_t v) {
                assert (cntBits_ < 5u);
                auto vv = (static_cast<uint16_t> (bits_) << 8u) | static_cast<uint16_t> (v);
                if (cntBits_ < 2) {
                    auto remain = (cntBits_ + 8u) - 5u;
                    *out++ = detail::encode (vv >> remain);
                    cntBits_ = remain;
                    bits_    = vv & detail::mask (remain);
                }
                else {
                    auto remain = (cntBits_ + 8u) - 10u;
                    *out++ = detail::encode (vv >> (5u + remain));
                    *out++ = detail::encode (vv >> remain);
                    cntBits_ = remain;
                    bits_    = vv & detail::mask (remain);
                }
            }

        template <typename OutIt_>
            void finalize (OutIt_ out) {
                if (0 < cntBits_) {
                    *out++ = detail::encode (bits_ << (5u - cntBits_));
                }
                cntBits_   = 0u;
                bits_      = 0u;
                finalized_ = 1u;
            }
    };

    template <typename InIt_, typename OutIt_>
        OutIt_ encode (InIt_ start, InIt_ end, OutIt_ out) {
            Encoder   enc;
            for (auto it = start ; it != end ; ++it) {
                enc.encode (out, static_cast<uint8_t> (*it));
            }
            enc.finalize (out);
            return out;
        }

    class Decoder {
        uint8_t bits_    = 0;
        uint8_t cntBits_ = 0;
    public:
        Decoder () = default;

        template <typename OutIt_>
            void operator() (OutIt_ out, uint8_t ch) {
                decode (out, ch);
            }

        template <typename OutIt_>
            bool decode (OutIt_ out, uint8_t ch) {
                auto v = detail::decode (ch);
                if (!v) {
                    return false;
                }
                auto vv    = static_cast<uint16_t> (v.value () & 0x1Fu) | (static_cast<uint16_t> (bits_) << 5u);
                auto nBits = cntBits_ + 5u;
                if (nBits < 8u) {
                    cntBits_ += 5u;
                    bits_ = static_cast<uint8_t> (vv);
                }
                else {
                    *out++ = static_cast<uint8_t> (vv >> (nBits - 8u));
                    cntBits_ = nBits - 8u;
                    bits_    = static_cast<uint8_t> (vv & detail::mask (cntBits_));
                }
                return true;
            }
    };

    template <typename InIt_, typename OutIt_>
        std::optional<OutIt_> decode (InIt_ start, InIt_ end, OutIt_ out) {
            Decoder   dec;
            for (auto it = start ; it != end ; ++it) {
                if (!dec.decode (out, static_cast<uint8_t> (*it))) {
                    return {};
                }
            }
            return { out };
        }
}
