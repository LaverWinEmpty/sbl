#ifndef SBL_HASH_TABLE_HEADER__
#define SBL_HASH_TABLE_HEADER__

#include "../source/array.hpp"

namespace sbl {

template<typename Key, typename Value, const SzInt BUCKET_AMOUNT = 31> class Dictionary
{
    DECLARE_NO_COPY(Dictionary);

public:
    Dictionary();
    ~Dictionary();

public:
    Array<Value>& operator[](IN const Key& key); // Get bucket

private:
    SzInt GetKey(IN const Key& key);

public:
    void   Insert(IN const Key& key, IN Value data);
    Value& Find(IN const Key& key);
    void   Remove(IN const Key& key, IN const Value& value);

private:
    Array<Value> buckets[BUCKET_AMOUNT];
};

} // namespace sbl
#endif