#ifndef SBL_CRYPT_HEADER__
#define SBL_CRYPT_HEADER__

/*
    AES256 => main
    BASE64 => after encrypt, before decrypt
    SHA256 => stronger security
*/

#include "../../security/include/aes256.h"
#include "../../security/include/sha256.h"
#include "../../utilities/include/base64.h"

namespace sbl {

class Crypter
{
    DECLARE_LIMIT_LIFECYCLE(Crypter);

public:
    static std::string Encoding(IN const Ptr ptr, IN SzInt size, IN const std::string& key);
    static std::string Encoding(IN const std::string& data, IN const std::string& key);

public:
    // ### PKCS#7 Padding ###
    // nullptr: auto delete
    // #### not null ####
    // - In ambiguous case: if the last value is 1
    // - Apart from that, if delete manually.
    static std::string Decoding(IN const std::string& data, IN const std::string& key, OUT OPT int* padding = nullptr);

public:
    static std::string Hashing(IN const std::string& data, IN const std::string& salt, IN SzInt stretching);
};

} // namespace sbl
#endif
