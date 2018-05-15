#pragma once

#include "Crypto\SecureContainer.h"
#include "Model\BasicTypes.h"

#include <memory>

namespace rail
{
    using SByteArray32UPtr = std::unique_ptr< SecureContainer< ByteArray32 > >;
}