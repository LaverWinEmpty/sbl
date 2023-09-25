// #ifndef SBL_HASH_TOOL_HEADER__
// #define SBL_HASH_TOOL_HEADER__

// #include "string"
// #include "../../security/include/sha256.h"
// #include "../../utilities/include/spooky2.h"

// namespace sbl {

// class Hasher
// {
// public:
//     template<typename T> static UInt32      ToHash32(IN T param);
//     template<typename T> static UInt64      ToHash64(IN T param);
//     template<typename T> static UInt128     ToHash128(IN T param);
//     template<typename T> static Block256    ToHash256(IN T param);
//     template<typename T> static std::string ToHash256String(IN T param);

// public:
//     static UInt32      ToHash32(IN Ptr ptr, SzInt size);
//     static UInt64      ToHash64(IN Ptr ptr, SzInt size);
//     static UInt128     ToHash128(IN Ptr ptr, SzInt size);
//     static Block256    ToHash256(IN Ptr ptr, SzInt size);
//     static std::string ToHash256String(IN Ptr ptr, SzInt size);
// };

// } // namespace sbl
// #endif