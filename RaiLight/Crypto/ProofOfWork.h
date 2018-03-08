#pragma once

#include "Model\BasicTypes.h"

static uint64_t const TEST_THRESHOLD = 0xff00000000000000;
static uint64_t const FULL_THRESHOLD = 0xffffffc000000000;
#ifdef TEST_MODE
static uint64_t const PUBLISH_THRESHOLD = TEST_THRESHOLD;
#else
static uint64_t const PUBLISH_THRESHOLD = TEST_THRESHOLD;
#endif

namespace rail
{
    namespace Pow
    {
        uint64_t generateWork(const ByteArray32& inputHash);
    };
}
