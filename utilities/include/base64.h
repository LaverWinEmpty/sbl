#ifndef SBL_BASE64_HEADER__
#define SBL_BASE64_HEADER__

#include "string"
#include "../../common/include/common.h"

namespace sbl {

class Base64
{
    DECLARE_LIMIT_LIFECYCLE(Base64);

private:
    static const char* TABLE;
    static int         ToIndex(IN char ascii);

public:
    static std::string Encoding(IN const std::string& str);
    static std::string Decoding(IN const std::string& str);
};

} // namespace sbl
#endif