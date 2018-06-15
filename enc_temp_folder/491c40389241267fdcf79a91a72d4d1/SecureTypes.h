#pragma once

#include "Crypto\SecureContainer.h"
#include "Model\BasicTypes.h"

#include <memory>

namespace rail
{
    using SByteArray32 = SecureContainer<ByteArray32> ;
    using SByteArray32UPtr = std::unique_ptr<SByteArray32> ;
}