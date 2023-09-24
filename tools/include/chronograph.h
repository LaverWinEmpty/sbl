#ifndef SBL_CHRONOGRAPH_HEADER__
#define SBL_CHRONOGRAPH_HEADER__

#include "chrono"
#include "../../common/include/common.h"

namespace sbl {

template<typename T> class Chronograph
{
public:
    Chronograph();

public:
    void Reset();
    T    Stop() const;

public:
    void UpdateDelta();
    T    GetDeltaTimeMS() const;
    T    GetDeltaTimeUS() const;
    T    GetDeltaTimeNS() const;

public:
    prop(get = GetDeltaTimeMS) T DeltaMS;
    prop(get = GetDeltaTimeUS) T DeltaUS;
    prop(get = GetDeltaTimeNS) T DeltaNS;

public:
    static T GetRunningTime();

public:
    static T SecFromMS(IN T param);
    static T SecFromUS(IN T param);
    static T SecFromNS(IN T param);

private:
    static const std::chrono::steady_clock::time_point run;

private:
    std::chrono::steady_clock::time_point          start;
    std::chrono::high_resolution_clock::time_point last; // delta

private:
    std::chrono::nanoseconds delta;
};

} // namespace sbl
#endif