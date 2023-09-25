#ifndef SBL_HASH_TABLE_SOURCE__
#define SBL_HASH_TABLE_SOURCE__

#include "../include/dictonary.h"
#include "../../utilities/include/crc32.h"

namespace sbl {

template<typename K, typename V, SzInt n> Dictionary<K, V, n>::Dictionary() {}

template<typename K, typename V, SzInt n> Dictionary<K, V, n>::~Dictionary() {}

template<typename K, typename V, SzInt n> SzInt Dictionary<K, V, n>::GetKey(IN const K& key)
{
    return CRC32::Castagnoli(&key, sizeof(K)) % n;
}

template<typename K, typename V, SzInt n> Array<V>& Dictionary<K, V, n>::operator[](IN const K& key)
{
    return buckets[GetKey(key)];
}

template<typename K, typename V, SzInt n> void Dictionary<K, V, n>::Insert(IN const K& key, IN V value)
{
    buckets[GetKey(key)].Push(value);
}

template<typename K, typename V, SzInt n> V& Dictionary<K, V, n>::Find(IN const K& key, IN SzInt index)
{
    SzInt bucket = GetKey(key);
    if(index >= buckets[bucket].Size) {
        throw ErrorBuilder::OutOfRange();
    }
    return buckets[bucket][index];
}

template<typename K, typename V, SzInt n> void Dictionary<K, V, n>::Remove(IN const K& key, IN const V& value)
{
    SzInt bucket = CRC32::Castagnoli(&key, sizeof(K));

    Array<V>* arr = buckets[bucket];
    for(int i = 0; i < arr->Size(); ++i) {
        if(arr[i] == value) {
            arr->Remove(value);
            return;
        }
    }
}

} // namespace sbl
#endif