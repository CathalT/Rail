#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include "Utilities\Conversions.h"

#include <QMessageLogger>
#include <QDebug>

#include <codecvt>

char const * account_lookup("13456789abcdefghijkmnopqrstuwxyz");
char const * account_reverse("~0~1234567~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~89:;<=>?@AB~CDEFGHIJK~LMNO~~~~~");

namespace rail
{
    char Conversions::encodeByte(const uint8_t value)
    {
        assert(value < 32);
        auto result(account_lookup[value]);
        return result;
    }

    uint8_t Conversions::decodeByte(const char value)
    {
        assert(value >= '0');
        assert(value <= '~');
        auto result(account_reverse[value - 0x30] - 0x30);
        return static_cast<uint8_t>(result);
    }

    utility::string_t Conversions::fromQString(const QString & qStr)
    {
        return toUtilString(qStr.toStdString());
    }

    std::string Conversions::uint64ToHexStr(uint64_t input)
    {
        std::stringstream stream;
        stream << std::hex << std::uppercase << std::noshowbase << std::setw(16) << std::setfill('0');
        stream << input;
        return stream.str();
    }

    std::optional<ByteArray32> Conversions::decodeHexFromString(const std::string& stdStr)
    {
        ByteArray32 outArray;

        if (!stdStr.empty() && stdStr.size() <= 64)
        {
            std::stringstream stream(stdStr);
            stream << std::hex << std::noshowbase;
            uInt256_t number_l;
            try
            {
                stream >> number_l;

                for (auto i(outArray.rbegin()), n(outArray.rend()); i != n; ++i)
                {
                    *i = static_cast<std::byte>(((number_l) & 0xff).convert_to<uint8_t>());
                    number_l >>= 8;
                }

                if (!stream.eof())
                {
                    outArray = {};
                }
            }
            catch (const std::exception & e)
            {
                QMessageLogger().warning() << e.what();
                outArray = {};
            }
        }

        return outArray;
    }

    std::string Conversions::toStdString(const utility::string_t& utilStr)
    {
#ifdef _WIN32
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.to_bytes(utilStr);
#else
        return utilStr;
#endif 
    }

    utility::string_t Conversions::toUtilString(const std::string& stdStr)
    {
#ifdef _WIN32
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        return converter.from_bytes(stdStr);
#else
        return stdStr;
#endif 
    }

    QString Conversions::toQString(const utility::string_t & utilStr)
    {
        QString returnVal;
#ifdef _WIN32
        returnVal = QString::fromStdWString(utilStr);
#else
        returnVal = QString::fromStdString(utilStr);
#endif
        return returnVal;
    }
}

