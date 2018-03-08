#pragma once

namespace rail
{
    namespace Conversions
    {
        template<typename Num, typename ByteArray>
        Num hexBytesToInt(const ByteArray& input)
        {
            Num outputInteger(0);
            auto shiftBy = 0;
            for (auto byte : input)
            {
                outputInteger <<= shiftBy;
                outputInteger |= byte;
                shiftBy = CHAR_BIT;
            }
            return outputInteger;
        }
        
        template<typename ByteArray, typename Num>
        ByteArray intToByteArray(Num input)
        {
            ByteArray outputArray;


            for (auto i(outputArray.rbegin()), n(outputArray.rend()); i != n; ++i)
            {
                *i = static_cast<ByteArray::value_type>(((input) & 0xff).convert_to<uint8_t>());
                input >>= 8;
            }

            return outputArray;
        }

        template<typename ByteArray, typename U>
        ByteArray uIntToByteArray(const U uInt)
        {
            ByteArray outputArray;
            auto copy = uInt;
            for (auto i(outputArray.rbegin()), n(outputArray.rend()); i != n; ++i)
            {
                *i = static_cast<ByteArray::value_type>((copy) & 0xff);
                copy >>= 8;
            }
            return outputArray;
        }

        template<typename Num>
        utility::string_t numberToUtilString(const Num num)
        {
            utility::string_t returnVal;
#ifdef _WIN32
            returnVal = std::to_wstring(num);
#else
            returnVal = std::to_string(num);
#endif
            return returnVal;
        }

        template<typename ByteArray>
        std::string encodeToHexStr(const ByteArray& input)
        {
            std::stringstream stream;
            const auto bitFieldWidth = input.size() * 2;

            stream << std::hex << std::noshowbase << std::setw(bitFieldWidth) << std::setfill('0');

            if (input.size() > 32)
            {
                stream << Conversions::hexBytesToInt<uInt512_t>(input);
            }
            else
            {
                stream << Conversions::hexBytesToInt<uInt256_t>(input);
            }

            return stream.str();
        }

        template <typename From, typename To>
        struct static_caster
        {
            To operator()(From p) { return static_cast<To>(p); }
        };

        template<typename ByteArray, typename BaseType, class V>
        ByteArray toByteArray(const V& v)
        {
            ByteArray output;
            assert(v.size() == output.size());
            using std::begin; using std::end;
            std::transform(begin(v), end(v), begin(output), static_caster<BaseType, ByteArray::value_type>());
            return output;
        }
    }
}
