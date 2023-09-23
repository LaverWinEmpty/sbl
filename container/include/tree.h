#ifndef __SEMIBASE_RED_BLACK_TREE_HEADER__
#define __SEMIBASE_RED_BLACK_TREE_HEADER__

#include "../source/node.hpp"

/*
    Tree
        Red-Black Tree

    ColorNode
        Only for tree

    Nil
        Only for color node
*/

namespace sbl {

template<typename T> class ColorNode;
template<typename T> using PtrClrNode = class ColorNode<T>*;

class Nil
{
    template<typename T> friend class ColorNode;
    template<typename T> friend class Iterator;

    template<typename T> static PtrClrNode<T> Address() { return RAW_CAST(PtrClrNode<int>, PtrClrNode<T>, &_nil); }

    static const ColorNode<int> _nil;
};

// Red Black Node
template<typename T> class ColorNode: public Node<T>
{
    friend class Nil;

public:
    enum EColor : bool
    {
        BLACK,
        RED
    };

    // For NIL node
    ColorNode(EColor color): Node<T>(), _color(color) { this->left = this->right = GetNil(); }

public:
    ColorNode(): Node<T>() { this->left = this->right = GetNil(); }
    ColorNode(IN const Node<T>& ref): Node<T>(ref) { this->left = this->right = GetNil(); }
    ColorNode(IN const T& arg): Node<T>(arg) { this->left = this->right = GetNil(); }
    ~ColorNode() {}

    ColorNode<T>& operator=(IN const T& arg) { return static_cast<ColorNode<T>&>(Node<T>::operator=(arg)); }
    ColorNode<T>& operator=(IN const Node<T>& ref) { return static_cast<ColorNode<T>&>(Node<T>::operator=(ref)); }
    ColorNode<T>& operator=(IN const ColorNode<T> ref) { return Node<T>::operator=(ref); }

public:
    static PtrClrNode<T> GetNil() { return Nil::Address<T>(); }

private:
    void DestroySubTreeRecursion();

public:
    void DestroySubTree();

public:
    bool IsRoot() { return _parent == nullptr; }
    bool IsLeft() { return _parent != nullptr && _parent->left == this; }
    bool IsRight() { return _parent != nullptr && _parent->right == this; }
    bool IsLeaf() { return this->left == GetNil() && this->right == GetNil(); }
    bool IsNull() { return this == nullptr || this == GetNil(); }
    bool IsNil() { return this == GetNil(); }

public:
    bool HasOneChild() { return this->Left->IsNull() == false |= this->Right->IsNull() == false; }
    bool HasTwoChildren() { return this->Left->IsNull() == false && this->Right->IsNull() == false; }

public:
    T*   GetDataAddress() { return &this->data; }
    T    GetData() { return this->data; }
    void SetData(IN const T& param) { this->data = param; }

public:
    PtrClrNode<T> GetLeft() { return static_cast<PtrClrNode<T>>(this->left); }
    void          SetLeft(IN PtrClrNode<T> param);

public:
    PtrClrNode<T> GetRight() { return static_cast<PtrClrNode<T>>(this->right); }
    void          SetRight(IN PtrClrNode<T> param);

public:
    PtrClrNode<T> GetParent() { return _parent; }
    void          SetParent(IN PtrClrNode<T> param) { _parent = param; }

public:
    PtrClrNode<T> GetGrandParent();
    PtrClrNode<T> GetSibling();
    PtrClrNode<T> GetUncle();

public:
    EColor GetColor() { return _color; }
    void   SetColor(EColor color) { _color = color; }

public:
    int GetLevel();

public:
    void SetBlack() { _color = EColor::BLACK; }
    void SetRed() { _color = EColor::RED; }
    bool IsBlack() { return _color == EColor::BLACK; }
    bool IsRed() { return _color == EColor::RED; }

public:
    prop(get = GetData, put = SetData) T Data;
    prop(get = GetColor, put = SetColor) EColor Color;
    prop(get = GetLeft, put = SetLeft) PtrClrNode<T> Left;
    prop(get = GetRight, put = SetRight) PtrClrNode<T> Right;
    prop(get = GetParent, put = SetParent) PtrClrNode<T> Parent;
    prop(get = GetGrandParent) PtrClrNode<T> GrandParent;
    prop(get = GetSibling) PtrClrNode<T> Sibling;
    prop(get = GetUncle) PtrClrNode<T> Uncle;
    prop(get = GetLevel) int Level;

private:
    // To Private
    using Node<T>::data;
    using Node<T>::left;
    using Node<T>::right;

private:
    PtrClrNode<T> _parent = nullptr;
    EColor        _color  = EColor::RED;
};

template<typename T> class Tree
{
public:
    template<typename Other> using PfnCompare = EComparison (*)(const T&, const Other&);

public:
    ~Tree() { _root->DestroySubTree(); }

public:
    using PfnOrder = void (*)(PtrClrNode<T>);

private:
    void RotateLeft(IN PtrClrNode<T> param);
    void RotateRight(IN PtrClrNode<T> param);
    void Transplant(IN PtrClrNode<T> param, IN PtrClrNode<T> old);

private:
    void FixInsert(IN PtrClrNode<T> param);
    void FixRemove(IN PtrClrNode<T> param);

public:
    PtrClrNode<T> FindMinimum(IN PtrClrNode<T> start);

public:
    T* Find(IN const T& ref);

public:
    template<typename Other> T* Find(IN const Other& ref, IN const PfnCompare<Other>);

private:
    PtrClrNode<T> Search(IN const T& ref);
    PtrClrNode<T> SearchForInsert(IN const T& ref);

public:
    bool Insert(IN const T& ref);
    bool Remove(IN const T& ref);

public:
    static int GetHeight(PtrClrNode<T> param);
    int        GetHeight();
    prop(get = GetHeight) int Height;

public:
    void Inorder(IN PfnOrder proc) { Tree<T>::Inorder(proc, _root); }
    void Preorder(IN PfnOrder proc) { Tree<T>::Preorder(proc, _root); }
    void Postorder(IN PfnOrder proc) { Tree<T>::Postorder(proc, _root); }
    void Levelorder(IN PfnOrder proc) { Tree<T>::Levelorder(proc, _root); }

public:
    static void Inorder(IN PfnOrder proc, IN PtrClrNode<T> node);
    static void Preorder(IN PfnOrder proc, IN PtrClrNode<T> node);
    static void Postorder(IN PfnOrder proc, IN PtrClrNode<T> node);
    static void Levelorder(IN PfnOrder proc, IN PtrClrNode<T> node);

public:
    PtrClrNode<T> Root() { return _root; }

private:
    PtrClrNode<T> _root = nullptr;
};

} // namespace sbl

#endif