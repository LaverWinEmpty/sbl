// #include "../include/hash.h"
// #include "../../security/include/sha256.h"
// #include "../../utilities/include/spooky2.h"

// namespace sbl {

// template<typename T> UInt32 Hasher::ToHash32(IN T param)
// {
//     return ToHash32(&param, sizeof(T));
// }

// template<typename T> UInt64 Hasher::ToHash64(IN T param)
// {
//     return ToHash64(&param, sizeof(T));
// }

// template<typename T> UInt128 Hasher::ToHash128(IN T param)
// {
//     return ToHash128(&param, sizeof(T));
// }

// template<typename T> Block256 Hasher::ToHash256(IN T param)
// {
//     return ToHash256(&param, sizeof(T));
// }

// template<typename T> std::string Hasher::ToHash256String(IN T param)
// {
//     return ToHash256String(&param, sizeof(T));
// }

// UInt32 Hasher::ToHash32(IN Ptr ptr, IN SzInt size)
// {
//     SpookyV2 hasher;
//     return hasher.Hash32(ptr, size);
// }

// UInt64 Hasher::ToHash64(IN Ptr ptr, IN SzInt size)
// {
//     SpookyV2 hasher;
//     return hasher.Hash64(ptr, size);
// }

// UInt128 Hasher::ToHash128(IN Ptr ptr, IN SzInt size)
// {
//     SpookyV2 hasher;
//     return hasher.Hash128(ptr, size);
// }

// Block256 Hasher::ToHash256(IN Ptr ptr, IN SzInt size)
// {
//     SHA256 hasher;
//     return hasher.Hash(ptr, static_cast<UWord>(size));
// }

// // Crypto hash
// std::string Hasher::ToHash256String(IN Ptr ptr, IN SzInt size)
// {
//     SHA256      hasher;
//     Block256    hash = hasher.Hash(ptr, static_cast<UWord>(size));
//     std::string result;
//     char        buffer[3];

//     for(int i = 0; i < 32; ++i) {
//         sprintf_s(buffer, "%02hhX", hash[i]);
//         result += buffer;
//     }

//     return result;
// }

// } // namespace sbl