#ifndef SBL_SHA256_HEADER__
#define SBL_SHA256_HEADER__

#include "string"
#include "../../common/include/common.h"
#include "../../common/include/block.h"

namespace sbl {

class SHA256
{
private:
    readonly SzInt DIGEST_BLOCK_LEN = 64;
    readonly SzInt DIGEST_VALUE_LEN = 32;

private:
    static const UWord CONSTANTS[64];

private:
    struct Info
    {
        UWord chainVar[DIGEST_VALUE_LEN >> 2];
        UWord highLength;
        UWord lowLength;
        UWord remainNum;
        UByte buffer[DIGEST_BLOCK_LEN];
    };

public:
    SHA256();

private:
    void     Initialize();
    void     Compress(IN Ptr message, IN UInt32 size);
    Block256 Final(IN Ptr message);

public:
    static std::string Hashing(IN const Ptr message, IN SzInt size);
    static std::string Hashing(IN const std::string msg);
    static std::string Encrypt(IN const Ptr message, IN SzInt messageSize, IN const Ptr salt, IN SzInt saltSize,
                               IN SzInt stretching);
    static std::string Encrypt(IN const std::string msg, IN const std::string salt, IN SzInt stretching);

private:
    void Transform(IN UInt32* msg, IN UInt32* chainVar);

private:
    static UInt32 RR(IN UInt32 x, UInt32 n);
    static UInt32 SS(IN UInt32 x, UInt32 n);
    static UInt32 Choose(IN UInt32 x, IN UInt32 y, IN UInt32 z);
    static UInt32 Majority(IN UInt32 x, IN UInt32 y, IN UInt32 z);
    static UInt32 Sigma0(IN UInt32 x);
    static UInt32 Sigma1(IN UInt32 x);
    static UInt32 RHO0(IN UInt32 x);
    static UInt32 RHO1(IN UInt32 x);

private:
    Info info;

private:
    static const bool IS_LITTLE_ENDIAN;
};

EXPORT char* SHA256_Hasing(IN const void* message, IN int size);
EXPORT char* SHA256_Encrypt(IN const void* message, IN int msgSize, IN const void* salt, IN int saltSize,
                            IN int stretching);

} // namespace sbl
#endif