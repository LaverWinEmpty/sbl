#ifndef __SEMIBASE_ITERATOR_HEADER__
#define __SEMIBASE_ITERATOR_HEADER__

#include "../source/node.hpp"

namespace sbl {

template<typename T> class Iterator
{
private:
    static bool IsNull(IN Node<T>* param);

public:
    bool IsNull();
    bool IsBegin();
    bool IsEnd();

public:
    bool MoveToParent();

public:
    Iterator();
    Iterator(IN Node<T>* param);

public:
    Node<T>* GetNode();
    void     SetNode(IN Node<T>* param);
    prop(get = GetNode, put = SetNode) Node<T>* node;

public:
    T& operator*();
    T* operator->();

public:
    Iterator& operator++();
    Iterator& operator--();
    Iterator  operator++(int);
    Iterator  operator--(int);

private:
    Node<T>* _node = nullptr;
};

} // namespace sbl
#endif