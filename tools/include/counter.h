#ifndef __SEMIBASE_EXTERNAL_COUNTER_HEADER__
#define __SEMIBASE_EXTERNAL_COUNTER_HEADER__

#include "../../common/include/common.h"

namespace sbl {

template<typename T> class Counter;
template<typename T> using PtrCtr = Counter<T>*;
using FloatCounter                = Counter<float>;
using PtrFltCtr                   = Counter<float>*;

template<typename T = float> class Counter
{
public:
    Counter();
    Counter(IN T& source, IN T before, IN T after, IN T unit);
    Counter(IN T* source, IN T before, IN T after, IN T unit);

public:
    void Initialize(IN T* source, IN T before, IN T after, IN T unit);
    void Initialize(IN T& source, IN T before, IN T after, IN T unit);
    void Update();
    void UpdateDynamic(IN T increase);
    void UpdateCoefficient(IN T coiefficient);

public:
    bool  IsComplete();
    T     GetBefore();
    T     GetAfter();
    SzInt GetOverlapped();

public:
    void SetUnit(IN T unit);
    void SetOverlapped(IN SzInt param);
    void AddOverlapped(IN SzInt param);
    void AddOverlappedFixedAfter(IN SzInt param);

public:
    prop(get = GetBefore) T Before;
    prop(get = GetAfter) T After;
    prop(put = SetUnit) T Unit;
    prop(get = GetOverlapped, put = SetOverlapped) SzInt Overlapped;

private:
    void PostUpdate();
    void PostSetOverlapped();

private:
    T*    source;     // Source pointer
    T     before;     // Source value
    T     after;      // Final value
    T     unit;       // Increase Unit
    T     increase;   // Increase when Update(): unit * overlapped
    T     total;      // To be incremented: after - before
    SzInt overlapped; // For thread safety when overlapping
    SzInt scale;      // For overapping and FIXED TARGET VALUE, default: 0
};

} // namespace sbl
#endif