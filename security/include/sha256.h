#ifndef SBL_SHA256_HEADER__
#define SBL_SHA256_HEADER__

/*
    has C language compatible procedures
    - SHA256_Hasing
    - SHA256_Encrypt
    must call free() before the scope ends.
*/

#ifdef __cplusplus

// clang-format off
#include "string"
#include "../../common/include/common.h"
// clang-format on

namespace sbl {

class SHA256
{
private:
    readonly ssize_t DIGEST_BLOCK_LEN = 64;
    readonly ssize_t DIGEST_VALUE_LEN = 32;

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
    void     Compress(IN Ptr message, IN uint32_t size);
    Block256 Final(IN Ptr message);

public:
    static std::string Hashing(IN const Ptr message, IN ssize_t size);
    static std::string Hashing(IN const std::string msg);
    static std::string Encrypt(IN const Ptr message, IN ssize_t messageSize, IN const Ptr salt, IN ssize_t saltSize,
                               IN ssize_t stretching);
    static std::string Encrypt(IN const std::string msg, IN const std::string salt, IN ssize_t stretching);

private:
    void Transform(IN uint32_t* msg, IN uint32_t* chainVar);

private:
    static uint32_t RR(IN uint32_t x, uint32_t n);
    static uint32_t SS(IN uint32_t x, uint32_t n);
    static uint32_t Choose(IN uint32_t x, IN uint32_t y, IN uint32_t z);
    static uint32_t Majority(IN uint32_t x, IN uint32_t y, IN uint32_t z);
    static uint32_t Sigma0(IN uint32_t x);
    static uint32_t Sigma1(IN uint32_t x);
    static uint32_t RHO0(IN uint32_t x);
    static uint32_t RHO1(IN uint32_t x);

private:
    Info info;

private:
    static const bool IS_LITTLE_ENDIAN;
};

EXPORT char* SHA256_Hasing(IN const void* message, IN int size);
EXPORT char* SHA256_Encrypt(IN const void* message, IN int msgSize, IN const void* salt, IN int saltSize,
                            IN int stretching);

} // namespace sbl

// clang-format off
#else
#ifndef IN
#    define IN
#endif
#ifndef OUT
#    define OUT
#endif
#ifndef OPT
#    define OPT
#endif
// clang-format on

char* SHA256_Hasing(IN const void* message, IN int size);
char* SHA256_Encrypt(IN const void* message, IN int msgSize, IN const void* salt, IN int saltSize, IN int stretching);
#endif

#endif