#ifndef SBL_DOUBLY_LINKED_LIST_SOURCE__
#define SBL_DOUBLY_LINKED_LIST_SOURCE__

#include "../include/list.h"

namespace sbl {

template<typename T> List<T>::List(): head(nullptr), tail(nullptr), size(0), recent(0), cache(nullptr) {}

template<typename T> List<T>::List(T data): head(nullptr), tail(nullptr), size(1)
{
    Node<T>* newNode = new Node<T>(data);
    if(newNode == nullptr) {
        size = 0;
        return;
    }

    head   = newNode;
    tail   = newNode;
    cache  = head;
    recent = 0;
}

template<typename T> List<T>::~List()
{
    Clear();
}

// Remember last address
// Throw ErrMsg
template<typename T> T& List<T>::operator[](IN int index)
{
    if(0 > index || index >= size) {
        throw ErrorBuilder::OutOfRange();
    }

    // Init
    if(index == 0) {
        cache  = head;
        recent = 0;
        return cache->data;
    }

    // Same index
    if(recent == index) {
        return cache->data;
    }

    Node<T>* temp = cache;

    // Get data at right
    if(recent < index) {
        for(int i = recent; i < index; ++i) {
            temp = temp->right;
        }
    }

    // Get data at left
    else {
        for(int i = recent; i > index; --i) {
            temp = temp->left;
        }
    }

    // Update
    recent = index;
    cache  = temp;

    return temp->data;
}

// nullptr: Not exist
template<typename T> template<typename U> T* List<T>::Find(IN const U& data, IN List::FnCompare<U> proc)
{
    Node<T>* curr = head;
    for(SzInt i = 0; i < size; ++i) {
        if(proc(curr->data, data) == true) {
            return &curr->data;
        }
        curr = curr->right;
    }
    return nullptr;
}

// -1: Not exist
template<typename T> template<typename U> SzInt List<T>::GetIndex(IN const U& data, IN List::FnCompare<U> proc)
{
    for(SzInt i = 0; i < size; ++i) {
        if(proc(this[i], data)) {
            return i;
        }
    }
    return -1;
}

// Throw: ErrMsg
template<typename T> void List<T>::AddDataForTheFirst(IN T data)
{
    Node<T>* newNode = new Node<T>;
    if(newNode == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    newNode->data = data;
    head          = newNode;
    tail          = newNode;
    cache         = head;
    recent        = 0;
    ++size;
}

// Throw: ErrMsg
template<typename T> void List<T>::AddDataForBegin(IN T data)
{
    Node<T>* newNode = new Node<T>(data);
    if(newNode == nullptr) {
        throw ErrorBuilder::NewFailed();
    }

    newNode->right = head;
    head->left     = newNode;
    head           = newNode; // Update first node

    UpdateCache(0);
    ++size;
}

// Throw: ErrMsg
template<typename T> void List<T>::AddDataForEnd(IN T data)
{
    Node<T>* newNode = new Node<T>;
    if(newNode == nullptr) {
        throw ErrorBuilder::NewFailed();
    }
    newNode->data = data;

    newNode->left = tail;
    tail->right   = newNode;
    tail          = newNode; // New last node

    UpdateCache(size);
    ++size;
}

// Throw MsgErr
template<typename T> void List<T>::ReleaseNode(IN Node<T>* deleteNode, IN int index)
{
    if(size == 0) {
        throw ErrorBuilder::OutOfRange();
    }

    // Left is null == delete node is first
    if(deleteNode->left != nullptr) {
        deleteNode->left->right = deleteNode->right;
    }
    else {
        head = deleteNode->right;
    }

    // Right is null == delete node is last
    if(deleteNode->right != nullptr) {
        deleteNode->right->left = deleteNode->left;
    }
    else {
        tail = deleteNode->left;
    }

    UpdateCache(index, deleteNode);
    SAFE_DELETE(deleteNode);

    if(--size == 0) {
        head = nullptr;
        tail = nullptr;
    }
}

template<typename T> SzInt List<T>::Size() const
{
    return size;
}

template<typename T> SzInt List<T>::LastIndex() const
{
    return size - 1;
}

template<typename T> bool List<T>::IsEmpty() const
{
    return size == 0;
}

// Throw: ErrMsg
template<typename T> void List<T>::Push(IN T data)
{
    if(size == 0) {
        return AddDataForTheFirst(data);
    }
    else {
        return AddDataForBegin(data);
    }
}

// Throw: ErrMsg
template<typename T> void List<T>::Add(IN T data)
{
    if(size == 0) return AddDataForTheFirst(data);
    else {
        return AddDataForEnd(data);
    }
}

// Throw ErrMsg
template<typename T> void List<T>::Add(IN T data, IN int index)
{

    if(0 > index || index > size) {
        throw ErrorBuilder::OutOfRange();
    }
    if(size == 0) {
        AddDataForTheFirst(data);
    }
    if(index == 0) {
        AddDataForBegin(data);
    }
    if(index == size) {
        AddDataForEnd(data);
    }
    // else

    Node<T>* newNode = new Node<T>(data);
    if(newNode == nullptr) {
        throw ErrorBuilder::NewFailed();
        return;
    }

    // Set position: [index] left
    Node<T>* temp;

    if(index < (size >> 1)) {
        temp = head;
        for(int i = 0; i < index; ++i) {
            temp = temp->right; // Find
        }
    }

    else // if (index >= (size >> 1))
    {
        temp = tail;
        for(int i = size - 1; i >= index; ++i) {
            temp = temp->left; // Find
        }
    }

    // From new node
    newNode->right    = temp;       // [New] -> [R]
    newNode->left     = temp->left; // [L] <- [New]
    temp->left->right = newNode;    // [L] -> [New]
    temp->left        = newNode;    // [New] <- [R]

    ++size;
    UpdateCache(index);
}

// Throw ErrMsg
template<typename T> void List<T>::Pop()
{
    Node<T>* temp = head;
    head          = head->right;
    ReleaseNode(temp, 0);
}

// Throw ErrMsg
template<typename T> void List<T>::Pull()
{
    Node<T>* temp = tail;
    tail          = tail->left;
    ReleaseNode(temp, LastIndex());
}

// Throw ErrMsg
template<typename T> void List<T>::RemoveAt(IN int index)
{
    if(index == 0) {
        return Pop();
    }
    if(index == LastIndex()) {
        return Pull();
    }

    Node<T>* deleteNode = head;
    for(int i = 0; i < index; ++i) {
        deleteNode = deleteNode->right;
    }
    ReleaseNode(deleteNode, index);
}

template<typename T> void List<T>::Remove(IN const T& find)
{
    int index = 0;
    for(Node<T>* temp = head; temp != nullptr; temp = temp->right) {
        if(temp->data == find) {
            ReleaseNode(temp, index);
            return;
        }
        ++index;
    }
}

template<typename T> void List<T>::RemoveAll(IN const T& find)
{
    Node<T>* del;
    Node<T>* temp = head;

    for(int i = 0; i < size; ++i) {
        if(temp->data == find) {
            del  = temp;
            temp = temp->right;
            ReleaseNode(del, i);
            --i;
        }

        else {
            temp = temp->right;
        }
    }
}

template<typename T> void List<T>::Clear()
{
    Node<T>* del;
    Node<T>* temp = head;

    while(temp != nullptr) {
        del  = temp;
        temp = temp->right;
        SAFE_DELETE(del);
    }

    size   = 0;
    head   = nullptr;
    tail   = nullptr;
    cache  = head;
    recent = 0;
}

// Used when the data order is changed (Add / Remove)
// Parameters: Node and Index to be modified
template<typename T> void List<T>::UpdateCache(IN int changedNodeIndex, IN Node<T>* deleteNodePtr)
{
    // Add
    if(deleteNodePtr == nullptr) {
        // Add Left (Push)
        if(changedNodeIndex <= recent) {
            ++recent;
        }
        return;
    }

    // else, Remove
    if(deleteNodePtr == cache) {
        // Delete node == Last node
        if(changedNodeIndex == LastIndex()) {
            cache = cache->left;
            --recent;
        }

        // Self-deletion: null => ptr move
        else {
            cache = cache->right;
        }
    }

    // Deleted prev node
    // Self-deletion: change the new data
    if(changedNodeIndex < recent) {
        --recent;
    }
}

template<typename T> void List<T>::Sort(IN FnCompare<T> proc)
{
    if(size <= 1) {
        return;
    }
    QuickSortRecursion(head, tail, 0, size - 1, proc);
    cache  = head;
    recent = 0;
}

// Throw ErrMsg
template<typename T> void List<T>::Sort(IN int min, IN int max, IN FnCompare<T> proc)
{
    if(size <= 1) {
        return;
    }

    if(min < 0 || max >= size) {
        throw SErrorBuilder::OutOfRange();
    }

    QuickSortRecursion(head, tail, min, max, proc);
    cache  = head;
    recent = 0;
}

template<typename T>
void List<T>::Partition(IN Node<T>* left, IN Node<T>* right, IN int groupIndexMin, IN int groupIndexMax,
                        IN Node<T>** pivotNodePtr, IN int* NewPivotIndex, IN FnCompare<T> proc)
{
    // Pivot
    T pivot = left->data;

    // Left
    Node<T>* leftNodePtr = left;
    int      leftIndex   = groupIndexMin;

    // Right
    Node<T>* rightNodePtr = right;
    int      rightIndex   = groupIndexMax;

    // Crossed
    while(leftIndex < rightIndex) {

        // Find the low value on the right
        while(proc(pivot, rightNodePtr->data) == true) {
            if(rightNodePtr->left != nullptr) {
                rightNodePtr = rightNodePtr->left;
                --rightIndex;
            }
        }

        // Find the high value on the left
        while(leftIndex < rightIndex && proc(pivot, leftNodePtr->data) == false) {
            leftNodePtr = leftNodePtr->right;
            ++leftIndex;
        }

        // Data swap
        Node<T> temp  = *leftNodePtr;
        *leftNodePtr  = *rightNodePtr;
        *rightNodePtr = temp;
    }

    // Swap the pivot and the low value
    *left        = *leftNodePtr;
    *leftNodePtr = pivot;

    *pivotNodePtr  = leftNodePtr;
    *NewPivotIndex = leftIndex;
}

template<typename T>
void List<T>::QuickSortRecursion(IN Node<T>* left, IN Node<T>* right, IN int groupIndexMin, IN int groupIndexMax,
                                 IN FnCompare<T> proc)
{
    // End recursive
    if(groupIndexMin >= groupIndexMax) {
        return;
    }

    // else
    int      newPivotIndex;
    Node<T>* newPivotNodePtr = nullptr;

    Partition(left, right, groupIndexMin, groupIndexMax, &newPivotNodePtr, &newPivotIndex, proc);

    QuickSortRecursion(left, newPivotNodePtr->left, groupIndexMin, newPivotIndex - 1, proc);   // Left
    QuickSortRecursion(newPivotNodePtr->right, right, newPivotIndex + 1, groupIndexMax, proc); // Right
}

} // namespace sbl
#endif