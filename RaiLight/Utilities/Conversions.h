#pragma once

#include "Model\BasicTypes.h"

#include <cpprest\details\basic_types.h>
#include <QString>
#include <optional>

namespace rail
{
    namespace Conversions
    {
        char                        encodeByte(const uint8_t value);
        uint8_t                     decodeByte(const char value);
        QString                     toQString(const utility::string_t & utilStr);
        utility::string_t           fromQString(const QString & qStr);
        utility::string_t           toUtilString(const std::string & stdStr);
        std::string                 toStdString(const utility::string_t & utilStr);
        std::string                 uint64ToHexStr(uint64_t input);
        std::optional<ByteArray32>  decodeHexFromString(const std::string& stdStr);
        std::optional<ByteArray16>  decodeDecimalFromString(const std::string& stdStr);

        template<typename Num, typename ByteArray>
        Num hexBytesToInt(const ByteArray& input);

        template<typename ByteArray, typename Num>
        ByteArray intToByteArray(Num input);

        template<typename ByteArray, typename U>
        ByteArray uIntToByteArray(const U uInt);

        template<typename Num>
        utility::string_t numberToUtilString(const Num num);

        template<typename ByteArray>
        std::string encodeToHexStr(const ByteArray& input);

        template<typename ByteArray, class V>
        ByteArray toByteArray(const V& v);
    };
}
#include "Utilities\ConversionsImpl.h"
