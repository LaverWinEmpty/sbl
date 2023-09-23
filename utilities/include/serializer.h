#ifndef __SEMIBASE_DATA_CONVERT_HEADER__
#define __SEMIBASE_DATA_CONVERT_HEADER__

#include <iostream>
#include <string>
#include "../../common/include/common.h"

namespace sbl {

interface ISerializable abstract
{
    virtual std::string Serialize()                        = 0;
    virtual void        Deserialize(IN const std::string&) = 0;
};

class Serializer
{
    DECLARE_LIMIT_LIFECYCLE(Serializer);

private:
    static std::string ToBinary(IN Byte byte);
    static std::string ToBinary(IN const Ptr ptr, IN size_t size);
    static Int8        FromBinary(IN const std::string& str);
    static void        FromBinary(IN const std::string& str, OUT Ptr ptr);

private:
    static std::string Serialization(IN const Ptr ptr, IN size_t size);
    static void        Deserialization(IN const std::string& str, IN size_t size, OUT Ptr ptr);

public:
    template<typename T> static std::string Serialize(IN const T& data);
    template<typename T> static T           Deserialize(IN const std::string& str);
    template<typename T> static std::string DataToBinary(IN const T& data);
    template<typename T> static T           BinaryToData(IN const std::string& str);
    template<> static std::string           DataToBinary(IN const std::string& str);
    template<> static std::string           BinaryToData(IN const std::string& str);
};

template<typename T> inline std::string Serializer::Serialize(IN const T& data)
{
    return Serialization(SVTableInspector<T>::SkipVTable(&data), SVTableInspector<T>::SizeWithoutVTable());
}

template<typename T> inline T Serializer::Deserialize(IN const std::string& str)
{
    T data;
    Deserialization(str, SVTableInspector<T>::SizeWithoutVTable(), SVTableInspector<T>::SkipVTable(&data));
    return data;
}

template<typename T> inline std::string Serializer::DataToBinary(IN const T& data)
{
    int size = SVTableInspector<T>::SizeWithoutVTable();
    return ToBinary(SVTableInspector<T>::SkipVTable(&data), SVTableInspector<T>::SizeWithoutVTable());
}

template<typename T> inline T Serializer::BinaryToData(IN const std::string& str)
{
    T data;
    FromBinary(str, SVTableInspector<T>::SkipVTable(&data));
    return data;
}

template<> inline std::string Serializer::DataToBinary(IN const std::string& str)
{
    return ToBinary(str.c_str(), str.size());
}

template<> inline std::string Serializer::BinaryToData(IN const std::string& str)
{
    char* buffer = new char[(str.size() >> 3) + 1]{ 0 };
    FromBinary(str, buffer);
    std::string res = buffer;
    delete[] buffer;
    return res;
}

} // namespace sbl
#endif