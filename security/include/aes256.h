#ifndef __SEMIBASE_AES_HEADER__
#define __SEMIBASE_AES_HEADER__

#include "string"
#include "../../common/include/common.h"
#include "../../common/include/block.h"

namespace sbl {

class AES256
{
public:
    readonly Int64 BLOCK_SIZE    = 16;                                    // AES
    readonly Int64 KEY_SIZE_BYTE = 32;                                    // AES-256
    readonly Int64 ROUND_COUNT   = 14;                                    // AES-256
    readonly Int64 EXPANDED_SIZE = BLOCK_SIZE * ROUND_COUNT + BLOCK_SIZE; // 240 Byte
    readonly Int64 ROW           = 4;                                     // Matrix row
    readonly Int64 COL           = 4;                                     // Matrix column

public:
    static std::string Encrypt(IN const std::string& data, IN const std::string& key);
    static std::string Decrypt(IN const std::string& data, IN const std::string& key, OUT OPT int* padding = nullptr);

public:
    void KeyScheduling(IN const Ptr key);

private:
    void Encrypt(IN OUT Ptr data);
    void Decrypt(IN OUT Ptr data);

private:
    UByte GaloisField(IN UByte l, IN UByte r);
    void  AddRoundKey(IN const UByte* roundKey);
    void  SubBytes();
    void  SubBytesInverse();
    void  ShiftRows();
    void  ShiftRowsInverse();
    void  MixColumns();
    void  MixColumnsInverse();

private:
    static const UByte S_BOX[];
    static const UByte S_BOX_INV[];
    static const UByte ROUND_CONSTANT[];
    static const UByte MIX_COLUMN_TABLE[ROW][COL];
    static const UByte MIX_COLUMN_TABLE_INV[ROW][COL];

    UByte    stateMatrix[ROW][COL] = { 0 };
    UByte    exKey[EXPANDED_SIZE]  = { 0 };
    Block256 key;
    // UByte key[KEY_SIZE_BYTE];
};

EXPORT char* AES256_Encrypt(IN const void* data, IN int size, IN const char* key);
EXPORT char* AES256_Decrypt(IN const void* data, IN int size, IN const char* key, OUT OPT int* padding);

} // namespace sbl
#endif