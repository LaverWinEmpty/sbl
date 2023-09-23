#ifndef __SEMIBASE_NODE_SOURCE__
#define __SEMIBASE_NODE_SOURCE__

#include "../include/node.h"

namespace sbl {

template<typename T> Node<T>::Node(IN const T& arg)
{
    data = arg;
}

template<typename T> Node<T>::Node(IN const Node<T>& ref)
{
    data = ref.data;
}

template<typename T> Node<T>& Node<T>::operator=(IN const T& arg)
{
    data = arg;
    return *this;
}

template<typename T> Node<T>& Node<T>::operator=(IN const Node<T>& ref)
{
    data = ref.data;
    return *this;
}

template<typename T> bool Node<T>::operator==(IN const Node<T>& ref) const
{
    return data == ref.data;
}

template<typename T> bool Node<T>::operator!=(IN const Node<T>& ref) const
{
    return data != ref.data;
}

template<typename T> bool Node<T>::operator>(IN const Node<T>& ref) const
{
    return data > ref.data;
}

template<typename T> bool Node<T>::operator<(IN const Node<T>& ref) const
{
    return data < ref.data;
}

template<typename T> bool Node<T>::operator>=(IN const Node<T>& ref) const
{
    return data >= ref.data;
}

template<typename T> bool Node<T>::operator<=(IN const Node<T>& ref) const
{
    return data <= ref.data;
}

} // namespace sbl
#endif