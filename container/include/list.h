#ifndef SBL_DOUBLY_LINKED_LIST_HEADER__
#define SBL_DOUBLY_LINKED_LIST_HEADER__

#include "../source/node.hpp"

/*
    List
        Doubly linked list

        ADT
            Add, Remove => List
            Push, Pop   => Like stack
            Push, Pull  => Like queue
            operator[]  => Like array
            Sort        => Quick sort
            Find        => Return Pointer

        Accessible by index
        When using remember last access address.
        If you access address 0, it will be initialized.
*/

namespace sbl {

template<typename T> class List
{
public:
    template<typename U> using FnCompare = bool (*)(const T&, const U&);

public:
    List();
    List(T data);
    ~List();

public:
    T& operator[](IN int index); // Find By Index

public:
    template<typename U> T*    Find(IN const U& data, IN FnCompare<U> proc = CompareEqual);
    template<typename U> SzInt GetIndex(IN const U& data, IN FnCompare<U> proc = CompareEqual);

public:
    SzInt Size() const;
    SzInt LastIndex() const;
    bool  IsEmpty() const;

public:
    void Push(IN T data); // To first
    void Add(IN T data);  // To last (default)
    void Add(IN T data, IN int index);

public:
    void Pop();                       // At head
    void Pull();                      // At tail
    void RemoveAt(IN int index);      // By index
    void Remove(IN const T& find);    // Find limit 1
    void RemoveAll(IN const T& find); // Find all
    void Clear();                     // All

public:
    void Sort(IN FnCompare<T> proc = CompareAscending);
    void Sort(IN int min, IN int max, IN FnCompare<T> proc = CompareAscending);

public:
    Node<T>* Begin() { return head; }
    Node<T>* End() { return tail; }
    Node<T>* Rear() { return head; }
    Node<T>* Front() { return tail; }
    Node<T>* Top() { return head; }
    Node<T>* Bottom() { return tail; }

private:
    void AddDataForTheFirst(IN T data);
    void AddDataForBegin(IN T data);
    void AddDataForEnd(IN T data);
    void ReleaseNode(IN Node<T>* deleteNode, IN int index);
    void UpdateCache(IN int index, IN OPT Node<T>* node = nullptr); // nullptr == no delete

private:
    void Partition(IN Node<T>*, IN Node<T>*, IN int, IN int, IN Node<T>**, IN int*, IN FnCompare<T>);
    void QuickSortRecursion(IN Node<T>*, IN Node<T>*, IN int, IN int, IN FnCompare<T>);

private:
    template<typename Other> static bool CompareEqual(IN const T& data, IN const Other& other) { return data == other; }
    static bool                          CompareAscending(IN const T& left, IN const T& right) { return left < right; }
    static bool                          CompareDescending(IN const T& left, IN const T& right) { return left > right; }

private:
    Node<T>* head;
    Node<T>* tail;
    SzInt    size;
    Node<T>* cache;  // Last accessed node pointer
    SzInt    recent; // Last accessed node index
};

} // namespace sbl
#endif