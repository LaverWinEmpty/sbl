#ifndef SBL_WINDOWS_WRAPPER_HEADER__
#define SBL_WINDOWS_WRAPPER_HEADER__

#include "windows.h"
#include "../../common/include/common.h"

namespace sbl {
namespace win {

class ErrorFormatter
{
private:
    readonly char* format = "%s (Code: %u)";

public:
    static std::string Formatting(DWORD errorCode);
};

} // namespace win
} // namespace sbl

#endif