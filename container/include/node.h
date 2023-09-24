#ifndef SEMIBASE_NODE_HEADER__
#define SEMIBASE_NODE_HEADER__

#include "../../common/include/common.h"

namespace sbl {

template<typename T> struct Node
{
    Node<T>* left  = nullptr;
    Node<T>* right = nullptr;
    T        data  = T();

    Node() {}
    virtual ~Node() {} // For ColorNode

    Node(IN const T& arg);
    Node(IN const Node<T>& ref);
    Node<T>& operator=(IN const T& arg);
    Node<T>& operator=(IN const Node<T>& ref);

    bool operator==(IN const Node<T>& ref) const;
    bool operator!=(IN const Node<T>& ref) const;
    bool operator>(IN const Node<T>& ref) const;
    bool operator<(IN const Node<T>& ref) const;
    bool operator>=(IN const Node<T>& ref) const;
    bool operator<=(IN const Node<T>& ref) const;
};

} // namespace sbl
#endif