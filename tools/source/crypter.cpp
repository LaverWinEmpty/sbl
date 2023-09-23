#include "../include/crypter.h"

namespace sbl {

std::string Crypter::Encoding(IN const Ptr ptr, IN SzInt size, IN const std::string& key)
{
    std::string str = std::string(static_cast<char*>(ptr), size);
    return Base64::Encoding(AES256::Encrypt(str, key));
}

std::string Crypter::Encoding(IN const std::string& data, IN const std::string& key)
{
    return Base64::Encoding(AES256::Encrypt(data, key));
}

std::string Crypter::Decoding(IN const std::string& data, IN const std::string& key, OUT OPT int* padding)
{
    return AES256::Decrypt(Base64::Decoding(data), key, padding);
}

} // namespace sbl