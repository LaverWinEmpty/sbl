#ifndef SEMIBASE_RED_BLACK_TREE_SOURCE_
#define SEMIBASE_RED_BLACK_TREE_SOURCE_

#include "../include/tree.h"
#include "array.hpp"

namespace sbl {

const ColorNode<int> Nil::_nil = ColorNode<int>(ColorNode<int>::BLACK);

template<typename T> void ColorNode<T>::DestroySubTreeRecursion()
{
    if(this->Left->IsNull() == false) {
        Left->DestroySubTreeRecursion();
    }

    if(this->Right->IsNull() == false) {
        Right->DestroySubTreeRecursion();
    }

    delete this;
}

template<typename T> void ColorNode<T>::DestroySubTree()
{
    if(this->IsNull() == true) {
        return;
    }

    if(IsRoot() == false) {
        if(IsLeft() == true) {
            _parent->Left = GetNil();
        }
        else _parent->Right = GetNil();
    }

    DestroySubTreeRecursion();
}

template<typename T> void ColorNode<T>::SetLeft(IN PtrClrNode<T> param)
{
    if(param != nullptr) {
        param->_parent = this;
    }
    this->left = static_cast<Node<T>*>(param);
}

template<typename T> void ColorNode<T>::SetRight(IN PtrClrNode<T> param)
{
    if(param != nullptr) {
        param->_parent = this;
    }
    this->right = static_cast<Node<T>*>(param);
}

template<typename T> PtrClrNode<T> ColorNode<T>::GetGrandParent()
{
    if(IsRoot() == true) {
        return nullptr;
    }
    return _parent->_parent;
}

template<typename T> PtrClrNode<T> ColorNode<T>::GetSibling()
{
    if(IsRoot() == true) {
        return nullptr;
    }
    if(IsLeft() == true) {
        return Parent->Right;
    }
    else return Parent->Left;
}

template<typename T> PtrClrNode<T> ColorNode<T>::GetUncle()
{
    PtrClrNode<T> p  = Parent;
    PtrClrNode<T> gp = GrandParent;
    if(gp == nullptr) {
        return nullptr;
    }
    if(p->IsLeft()) {
        return gp->Right;
    }
    else {
        return gp->Left;
    }
}

template<typename T> int ColorNode<T>::GetLevel()
{
    if(this == nullptr) {
        return 0;
    }

    return 1 + _parent->GetLevel();
}

template<typename T> void Tree<T>::RotateLeft(IN PtrClrNode<T> param)
{
    //  P         // Parent
    //   ¢Ù
    //      C     // Current (param)
    //   ¢×   ¢Ù
    //  L       R // Left and Right

    PtrClrNode<T> r = param->Right;

    //  P
    //   ¢Ù
    //      C
    //   ¢×   ¢Ù
    //  L       RL ¡ç R ¡æ RR // Right's Left and Right's Right

    param->Right = r->Left;
    if(r->Left->IsNil() == false) {
        r->Left->Parent = param;
    }

    //     P
    //   ¢× ¢Ù
    //  C     R // To Sibling (or root)
    //  ¡é¢Ù ¢×
    //  L  RL

    r->Parent = param->Parent;
    if(param->Parent->IsNull() == true) {
        _root = r;
    }
    else if(param->IsLeft() == true) {
        param->Parent->Left = r;
    }
    else {
        param->Parent->Right = r;
    }

    //  P
    //   ¢Ù
    //      R
    //   ¢×   ¢Ù
    //  C       RR
    //  ¡é¢Ù
    //  L  RL

    r->Left       = param;
    param->Parent = r;
}

template<typename T> void Tree<T>::RotateRight(IN PtrClrNode<T> param)
{
    // Same as RotateLeft, but in reverse

    PtrClrNode<T> l = param->Left;

    param->Left = l->Right;
    if(l->Right->IsNil() == false) {
        l->Right->Parent = param;
    }

    l->Parent = param->Parent;
    if(param->Parent->IsNull() == true) {
        _root = l;
    }
    else if(param->IsRight() == true) {
        param->Parent->Right = l;
    }
    else {
        param->Parent->Left = l;
    }

    l->Right      = param;
    param->Parent = l;
}

template<typename T> void Tree<T>::Transplant(IN PtrClrNode<T> param, IN PtrClrNode<T> old)
{
    // N: Any node
    // P: Param
    // O: Old => remove

    // R    R
    // ¦¢ => ¦¢
    // O    P

    if(old->IsRoot() == true) {
        _root = param;
    }

    else if(old->IsLeft() == true) {
        old->Parent->Left = param;
    }

    else {
        old->Parent->Right = param; // old is Right
    }

    if(old->IsNull() == false) {
        param->Parent = old->Parent;
    }
}

template<typename T> void Tree<T>::FixInsert(IN PtrClrNode<T> param)
{
    PtrClrNode<T> p, gp, u;

    while(param->IsRoot() == false && param->IsRed() == true && param->Parent->IsRed() == true) {
        p  = param->Parent;
        gp = param->GrandParent;
        u  = param->Uncle;

        // Uncle is Red: Recoloring
        if(u->IsNull() == false && u->IsRed() == true) {
            p->SetBlack();
            u->SetBlack();
            gp->SetRed();
            param = gp;
            continue;
        }

        // Uncle is Black: Restructuring
        if(p->IsLeft() == true) {
            if(param->IsRight() == true) {
                param = p;
                RotateLeft(p);
            }

            p->SetBlack();
            gp->SetRed();
            RotateRight(gp);
        }

        // Right: Reverse
        else {
            if(param->IsLeft() == true) {
                param = p;
                RotateRight(param);
            }

            p->SetBlack();
            gp->SetRed();
            RotateLeft(gp);
        }
    }

    _root->SetBlack();
}

template<typename T> void Tree<T>::FixRemove(IN PtrClrNode<T> param)
{
    PtrClrNode<T> s, p;

    while((param->IsNull() == true || param->IsBlack() == true) && (param != _root)) {
        s = param->Sibling;
        p = param->Parent;

        // Left
        if(param->IsLeft() == true) {
            // Sibling is Red
            if(s->IsRed() == true) {
                s->SetBlack();
                p->SetRed();
                RotateLeft(p);
                s = p->Right;
            }

            // All child nodes of sibling is Black
            else if(s->Left->IsBlack() && s->Right->IsBlack()) {
                s->SetRed();
                param = p;
                p     = param->Parent;
            }

            else {
                // Right node of sibling is Black
                if(s->Right->IsRed() == true) {
                    s->Left->SetBlack();
                    s->SetRed();
                    RotateRight(s);
                    s = p->Left;
                }

                s->Color = p->Color;
                p->SetBlack();
                s->Right->SetBlack();
                RotateLeft(p);
                param = _root;
                break;
            }
        }

        // Right
        else {
            // Sibling is Red
            if(s->IsRed() == true) {
                s->SetBlack();
                p->SetRed();
                RotateRight(p);
                s = p->Left;
            }

            // All child nodes of sibling is Black
            else if(s->Left->IsBlack() == true && s->Right->IsBlack() == true) {
                s->SetRed();
                param = p;
                p     = param->Parent;
            }

            else {
                // Left node of sibling is Black
                if(s->Left->IsRed() == true) {
                    s->Right->SetBlack();
                    s->SetRed();
                    RotateLeft(s);
                    s = p->Left;
                }

                s->Color = p->Color;
                p->SetBlack();
                s->Left->SetBlack();
                param = _root;
                break;
            }
        }
    }

    if(param->IsNull() == false) {
        param->SetBlack();
    }
}

template<typename T> PtrClrNode<T> Tree<T>::FindMinimum(IN PtrClrNode<T> cursor)
{
    while(cursor->Left->IsNull() == false) {
        cursor = cursor->Left;
    }
    return cursor;
}

// ONLY USE WHEN USING PAIR
// This method is for changing the VALUE only
// Do not modify the KEY
// nullptr: Not exist
template<typename T> T* Tree<T>::Find(IN const T& ref)
{
    PtrClrNode<T> n = Search(ref);
    if(n == nullptr) {
        return nullptr;
    }
    return n->GetDataAddress();
}

// nullptr: Not exist
template<typename T> template<typename Other> T* Tree<T>::Find(IN const Other& other, IN const PfnCompare<Other> fnComp)
{
    PtrClrNode<T> cursor = _root;
    while(cursor->IsNull() == false) {
        switch(PfnCompare(cursor->data)) {
            case EComparison::LESS: cursor = cursor->Left;
            case EComparison::EQUAL: return cursor->GetDataAddress;
            case EComparison::MORE: cursor = cursor->Right;
        }
    }
    return nullptr;
}

// nullptr: Not exist
template<typename T> PtrClrNode<T> Tree<T>::Search(IN const T& ref)
{
    PtrClrNode<T> cursor = _root;
    while(cursor->IsNull() == false) {
        if(ref < cursor->Data) {
            cursor = cursor->Left;
        }
        else if(ref > cursor->Data) {
            cursor = cursor->Right;
        }
        else return cursor;
    }

    return nullptr;
}

// nullptr: Exist
template<typename T> PtrClrNode<T> Tree<T>::SearchForInsert(const T& ref)
{
    PtrClrNode<T> cursor = _root;
    PtrClrNode<T> parent = nullptr;

    while(cursor->IsNull() == false) {
        parent = cursor;

        if(ref < cursor->Data) {
            cursor = cursor->Left;
        }
        else if(ref > cursor->Data) {
            cursor = cursor->Right;
        }
        else return nullptr;
    }

    return parent;
}

// true: Succeed
template<typename T> bool Tree<T>::Insert(const T& ref)
{
    PtrClrNode<T> newNode = new ColorNode<T>(ref);

    if(_root->IsNull() == true) {
        _root = newNode;
        _root->SetBlack();
        return true;
    }

    PtrClrNode<T> parent = SearchForInsert(ref);
    if(parent->IsNull() == true) {
        return false;
    }

    if(newNode->Data < parent->Data) {
        parent->Left = newNode;
    }
    else if(newNode->Data > parent->Data) {
        parent->Right = newNode;
    }
    else {
        // ?
        delete newNode;
        return false;
    }

    FixInsert(newNode);
    return true;
}

template<typename T> bool Tree<T>::Remove(const T& ref)
{
    PtrClrNode<T> node, child;

    node = Search(ref);
    if(node == nullptr) {
        return false;
    }

    // The minimum value in the right subtree
    // This node is moved to the location of the node to be deleted.
    PtrClrNode<T> min = FindMinimum(node->Right);

    // Node's children is full
    if(node->HasTwoChildren() == true) {

        child = min->Right;
        // The minimum value in the right subtree == Child
        if(min->Parent == node) {
            min->Parent = node->Parent;
        }

        else {
            // min's R -> min (unlinked)
            Transplant(min->Right, min);
            min->Right = node->Right;
            if(min->Right->IsNull() == false) {
                min->Right->Parent = min;
            }
            min->Parent = node->Parent;
        }

        Transplant(min, node); // min -> node to delete
        min->Left = node->Left;
        if(min->Left->IsNull() == false) {
            min->Left->Parent = min;
        }

        min->Color = node->Color;
    }

    // Node's child is 0 or 1
    // Has only the Right
    else if(node->Left->IsNull() == true) {
        child = node->Right;
        Transplant(node->Right, node);
    }

    // Has only the Left
    else {
        child = node->Left;
        Transplant(node->Left, node);
    }

    if(min->IsBlack() == true) {
        FixRemove(child);
    }

    delete node;
    return true;
}

template<typename T> int Tree<T>::GetHeight(IN PtrClrNode<T> param)
{
    if(param->IsNull() == true) {
        return 0;
    }

    int left  = GetHeight(param->Left);
    int right = GetHeight(param->Right);

    return 1 + MAX(left, right);
}

template<typename T> int Tree<T>::GetHeight()
{
    return Tree<T>::GetHeight(_root);
}

template<typename T> void Tree<T>::Inorder(IN PfnOrder proc, IN PtrClrNode<T> node)
{
    if(node->IsNull() == true) {
        return;
    }

    Inorder(proc, node->Left);
    proc(node);
    Inorder(proc, node->Right);
}

template<typename T> void Tree<T>::Preorder(IN PfnOrder proc, IN PtrClrNode<T> node)
{
    if(node->IsNull() == true) {
        return;
    }

    proc(node);
    Preorder(proc, node->Left);
    Preorder(proc, node->Right);
}

template<typename T> void Tree<T>::Postorder(IN PfnOrder proc, IN PtrClrNode<T> node)
{
    if(node->IsNull() == true) {
        return;
    }

    Postorder(proc, node->Left);
    Postorder(proc, node->Right);
    proc(node);
}

template<typename T> void Tree<T>::Levelorder(IN PfnOrder proc, IN PtrClrNode<T> node)
{
    if(node->IsNull() == true) {
        return;
    }

    Queue<PtrClrNode<T>> q;

    q.Enque(node);

    while(q.IsEmpty() == false) {
        PtrClrNode<T> node = q.Front();
        q.Deque();

        proc(node);

        if(node->Left->IsNil() == false) {
            q.Enque(node->Left);
        }

        if(node->Right->IsNil() == false) {
            q.Enque(node->Right);
        }
    }
}

} // namespace sbl

#endif