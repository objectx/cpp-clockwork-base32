//
// Copyright (c) 2020 Masashi Fujita. All rights reserved.
//
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
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

        static constexpr size_t  symbolBits = 5u;
        static constexpr uint8_t mask (size_t n) { return static_cast<uint8_t> ((1u << n) - 1u); }

        static constexpr uint8_t encode (uint16_t x) { return detail::symEncode[x & mask (symbolBits)]; }

        static constexpr std::optional<uint8_t> decode (uint8_t ch) {
            auto v = detail::symDecode[ch];
            if (v < (1u << symbolBits)) {
                return {v};
            }
            return {};
        }
    }  // namespace detail

    /// @brief The Clockwork Base32 encoder.
    template<typename OutIt_>
    class Encoder {
        OutIt_  sink_;
        uint8_t bits_      = 0;
        uint8_t cntBits_   = 0;
        uint8_t finalized_ = 0;

    public:
        ~Encoder () { finalize (); }

        explicit Encoder (OutIt_ sink) : sink_ {sink} { /* NO-OP */
        }

        [[nodiscard]] bool finalized () const { return finalized_ != 0; }

        OutIt_ sink () { return sink_; }

        Encoder &operator() (uint8_t v) { return encode (v); }

        /// @brief Encode supplied 1 octet.
        /// @param v Octet to encode
        Encoder &encode (uint8_t v) {
            assert (! finalized ());
            assert (cntBits_ < detail::symbolBits);

            auto constexpr uint8_digits = std::numeric_limits<uint8_t>::digits;

            uint16_t vv = (static_cast<uint16_t> (bits_) << uint8_digits) | static_cast<uint16_t> (v);
            // `vv` holds `cntBits_ + 8` bits information.
            // cntBits_ + 8 < 10 -> cntBits_ < 2
            if (cntBits_ < 2) {
                auto remain = (cntBits_ + uint8_digits) - detail::symbolBits;
                *sink_++    = detail::encode (vv >> remain);
                cntBits_    = remain;
                bits_       = vv & detail::mask (remain);
            }
            else {
                auto remain = (cntBits_ + uint8_digits) - (2u * detail::symbolBits);
                *sink_++    = detail::encode (vv >> (detail::symbolBits + remain));
                *sink_++    = detail::encode (vv >> remain);
                cntBits_    = remain;
                bits_       = vv & detail::mask (remain);
            }
            return *this;
        }

        template<typename InIt_>
        Encoder &operator() (InIt_ start, InIt_ end) {
            return encode (start, end);
        }

        /// @brief Encodes sequence.
        /// @tparam InIt_ The input iterator type
        /// @param start Start of the sequence
        /// @param end End of the sequence
        template<typename InIt_>
        Encoder &encode (InIt_ start, InIt_ end) {
            for (auto it = start; it != end; ++it) {
                encode (*it);
            }
            return *this;
        }

        /// @brief Finalize the output.
        void finalize () {
            if (! finalized ()) {
                if (0 < cntBits_) {
                    *sink_++ = detail::encode (bits_ << (detail::symbolBits - cntBits_));
                }
                cntBits_   = 0u;
                bits_      = 0u;
                finalized_ = 1u;
            }
        }
    }; /* class Encoder */

    /// @brief Encoding entire sequence at once.
    /// @tparam InIt_ The input iterator type
    /// @tparam OutIt_ The output iterator type
    /// @param start Start of the sequence
    /// @param end End of the sequence
    /// @param out The output
    /// @return Iterator pointed next to the last output.
    template<typename InIt_, typename OutIt_>
    OutIt_ encode (InIt_ start, InIt_ end, OutIt_ out) {
        Encoder enc {out};
        enc.encode (start, end);
        enc.finalize ();
        return out;
    }

    /// @brief The Clockwork Base32 decoder.
    template<typename OutIt_>
    class Decoder {
        OutIt_  sink_;
        uint8_t bits_    = 0;
        uint8_t cntBits_ = 0;

    public:
        explicit Decoder (OutIt_ sink) : sink_ {sink} { /* NO-OP */
        }

        OutIt_ sink () { return sink_; }

        bool operator() (uint8_t ch) { return decode (ch); }

        /// @brief Decodes the supplied symbol.
        /// @param ch Input symbol
        /// @return `true` if the supplied symbol is acceptable.
        bool decode (uint8_t ch) {
            auto v = detail::decode (ch);
            if (! v) {
                return false;
            }
            auto constexpr uint8_digits = std::numeric_limits<uint8_t>::digits;

            uint16_t vv = static_cast<uint16_t> (v.value () & detail::mask (detail::symbolBits))
                          | (static_cast<uint16_t> (bits_) << detail::symbolBits);
            auto nBits = cntBits_ + detail::symbolBits;
            if (nBits < uint8_digits) {
                cntBits_ += detail::symbolBits;
                bits_ = static_cast<uint8_t> (vv);
            }
            else {
                *sink_++ = static_cast<uint8_t> (vv >> (nBits - uint8_digits));
                cntBits_ = nBits - uint8_digits;
                bits_    = static_cast<uint8_t> (vv & detail::mask (cntBits_));
            }
            return true;
        }

        template<typename InIt_>
        InIt_ operator() (InIt_ start, InIt_ end) {
            return decode (start, end);
        }

        /// @brief Decodes supplied sequence.
        /// @tparam InIt_ The input iterator type
        /// @param start Start of the sequence
        /// @param end End of the sequence
        /// @return When all of inputs are valid, returns `end`.
        ///         Otherwise return the iterator point a symbol caused error.
        template<typename InIt_>
        InIt_ decode (InIt_ start, InIt_ end) {
            for (auto it = start; it != end; ++it) {
                if (! decode (*it)) {
                    return it;
                }
            }
            return end;
        }
    }; /* class Decoder */

    /// @brief Decodes the input sequence at once.
    /// @tparam InIt_ The input iterator type
    /// @tparam OutIt_ The output iterator type
    /// @param start Start of the sequence
    /// @param end End of the sequence
    /// @param out The output
    /// @return When all of inputs are valid, returns `end`.
    ///         Otherwise return the iterator point a symbol caused error.
    template<typename InIt_, typename OutIt_>
    InIt_ decode (InIt_ start, InIt_ end, OutIt_ out) {
        Decoder dec {out};
        return dec.decode (start, end);
    }
} /* namespace ClockworkBase32 */
