#include "RaiLight\Platform\WinUtils.h"

#include <windows.h>

void WinUtils::ThreadUtils::setThreadBackgroundAffinity()
{
    SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN);
}