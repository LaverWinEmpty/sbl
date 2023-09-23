#ifndef __SEMIBASE_ITERATOR_SOURCE__
#define __SEMIBASE_ITERATOR_SOURCE__

#include "../include/iterator.h"
#include "../include/tree.h"

namespace sbl {

// Compatible with ColorNode
template<typename T> bool Iterator<T>::IsNull(IN Node<T>* param)
{
    return param == nullptr || param == Nil::Address<T>();
}

template<typename T> bool Iterator<T>::IsNull()
{
    return IsNull(_node);
}

template<typename T> bool Iterator<T>::IsBegin()
{
    return IsNull(_node->left);
}

template<typename T> bool Iterator<T>::IsEnd()
{
    return IsNull(_node->right);
}

template<typename T> bool Iterator<T>::MoveToParent()
{
    PtrClrNode<T> n = dynamic_cast<PtrClrNode<T>>(_node);
    if(n == nullptr) {
        return false;
    }

    if(n->IsRoot() == true) {
        return false;
    }

    _node = static_cast<Node<T>*>(n);
    return true;
}

template<typename T> Iterator<T>::Iterator() {}

template<typename T> Iterator<T>::Iterator(IN Node<T>* param)
{
    _node = param;
}

template<typename T> Node<T>* Iterator<T>::GetNode()
{
    return _node;
}

template<typename T> void Iterator<T>::SetNode(IN Node<T>* param)
{
    _node = param;
}

template<typename T> T& Iterator<T>::operator*()
{
    return _node->data;
}

template<typename T> T* Iterator<T>::operator->()
{
    return &_node->data;
}

template<typename T> Iterator<T>& Iterator<T>::operator++()
{
    if(IsNull() == false && IsNull(_node->right) == false) {
        _node = _node->right;
    }
    return *this;
}

template<typename T> Iterator<T>& Iterator<T>::operator--()
{
    if(IsNull() == false && IsNull(_node->left) == false) {
        _node = _node->left;
    }
    return *this;
}

template<typename T> Iterator<T> Iterator<T>::operator++(int)
{
    Iterator i = _node;
    if(IsNull() == false && IsNull(_node->right) == false) {
        _node = _node->right;
    }
    return i;
}

template<typename T> Iterator<T> Iterator<T>::operator--(int)
{
    Iterator i = _node;
    if(IsNull() == false && IsNull(_node->left) == false) {
        _node = _node->left;
    }
    return i;
}

} // namespace sbl
#endif