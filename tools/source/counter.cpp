#include "../include/counter.h"

namespace sbl {

template<typename T>
Counter<T>::Counter():
    source(nullptr), before(T()), after(T()), unit(T()), total(T()), increase(T()), overlapped(1), scale(0)
{}

template<typename T>
Counter<T>::Counter(IN T& source, IN T before, IN T after, IN T unit):
    source(&source), before(before), after(after), unit(unit), total(after - before), increase(unit), overlapped(1),
    scale(0)
{
    source = before;
}

template<typename T>
Counter<T>::Counter(IN T* source, IN T before, IN T after, IN T unit):
    source(source), before(before), after(after), unit(unit), total(after - before), increase(unit), overlapped(1),
    scale(0)
{
    *source = before;
}

template<typename T> void Counter<T>::Initialize(IN T* sourceArg, IN T beforeArg, IN T afterArg, IN T unitArg)
{
    *this = Counter(sourceArg, beforeArg, afterArg, unitArg);
}

template<typename T> void Counter<T>::Initialize(IN T& sourceArg, IN T beforeArg, IN T afterArg, IN T unitArg)
{
    *this = Counter(sourceArg, beforeArg, afterArg, unitArg);
}

template<typename T> void Counter<T>::Update()
{
    if(source == nullptr) {
        return;
    }
    *source += increase;
    PostUpdate();
}

template<typename T> void Counter<T>::UpdateDynamic(IN T increase)
{
    if(source == nullptr) {
        return;
    }
    *source += (increase * overlapped);
    PostUpdate();
}

template<typename T> void Counter<T>::UpdateCoefficient(IN T coefficient)
{
    if(source == nullptr) {
        return;
    }
    *source += (increase * coefficient);
    PostUpdate();
}

template<typename T> bool Counter<T>::IsComplete()
{
    return source == nullptr;
}

template<typename T> T Counter<T>::GetBefore()
{
    return before;
}

template<typename T> T Counter<T>::GetAfter()
{
    return after;
}

template<typename T> SzInt Counter<T>::GetOverlapped()
{
    return overlapped;
}

template<typename T> void Counter<T>::SetUnit(IN T unitArg)
{
    unit     = unitArg;
    increase = unit * overlapped;
}

template<typename T> void Counter<T>::SetOverlapped(IN SzInt param)
{
    overlapped = param;
    PostSetOverlapped();
}

template<typename T> void Counter<T>::AddOverlapped(IN SzInt param)
{
    overlapped += param;
    PostSetOverlapped();
}

template<typename T> void Counter<T>::AddOverlappedFixedAfter(IN SzInt param)
{
    overlapped += param;
    scale += param;
    PostSetOverlapped();
}

template<typename T> void Counter<T>::PostUpdate()
{
    if(*source < after) {
        return;
    }

    *source = after;
    source  = nullptr;
}

template<typename T> void Counter<T>::PostSetOverlapped()
{
    after    = before + (total * (overlapped - scale));
    increase = unit * overlapped;
}

} // namespace sbl