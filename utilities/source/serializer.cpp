#include "../include/serializer.h"

namespace sbl {

std::string Serializer::Serialization(IN const Ptr ptr, IN size_t size)
{
    Byte*       cursor = ptr;
    std::string result;

    FAST_LOOP(size, result.push_back(*cursor++));
    return result;
}

// Throw ErrMsg
void Serializer::Deserialization(IN const std::string& str, IN size_t size, OUT Ptr ptr)
{
    if(str.size() != size) {
        throw ErrorBuilder::FormatError();
    }

    Byte* cursor = ptr;

    int64_t index = 0;
    FAST_LOOP(size, cursor[index] = str[index]; ++index);
}

// 1Byte
std::string Serializer::ToBinary(IN Byte byte)
{
    std::string result;

    Byte bit = 0b10000000;
    FAST_LOOP(8, result.push_back('0' + !!(byte & bit)); bit >>= 1);

    return result;
}

// Pointer
std::string Serializer::ToBinary(IN const Ptr ptr, IN size_t size)
{
    std::string result;
    Byte*       cursor = ptr;

    FAST_LOOP(size, result += ToBinary(*cursor++));
    return result;
}

// 1Byte
// Throw ErrMsg
Byte Serializer::FromBinary(IN const std::string& str)
{
    if(str.size() % 8) {
        throw ErrorBuilder::FormatError();
    }

    int result = 0;
    int index  = 0;
    FAST_LOOP(8, result |= ((str[index] == '1') << (7 - index)); ++index);
    return result;
}

// Pointer
// Throw ErrMsg
void Serializer::FromBinary(IN const std::string& str, OUT Ptr ptr)
{
    if(str.size() % 8) {
        throw ErrorBuilder::FormatError();
    }

    Byte* cursor = ptr;

    int64_t loop  = str.size();
    int64_t index = 0;
    FAST_LOOP(loop >> 3, cursor[index] = FromBinary(str.substr(index << 3, 8)); ++index);
}

} // namespace sbl