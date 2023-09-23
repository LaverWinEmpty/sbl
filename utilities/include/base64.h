#ifndef __SEMIBASE_DATA_CONVERT_HEADER__
#define __SEMIBASE_DATA_CONVERT_HEADER__

#include "string"
#include "../../common/include/common.h"

namespace sbl {

class Base64
{
    DECLARE_LIMIT_LIFECYCLE(Base64);

private:
    static const AChar* TABLE;
    static int          ToIndex(IN AChar ascii);

public:
    static std::string Encoding(IN const std::string& str);
    static std::string Decoding(IN const std::string& str);
};

} // namespace sbl
#endif