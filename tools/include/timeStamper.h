#ifndef __SEMIBASE_TIME_STAMPER_HEADER__
#define __SEMIBASE_TIME_STAMPER_HEADER__

#include "ctime"
#include "string"
#include "../../common/include/common.h"

namespace sbl {
class TimeStamper
{
public:
    DECLARE_NO_COPY(TimeStamper);

public:
    enum EOrder
    {
        ORDER_YYYY_MM_DD,
        ORDER_MM_DD_YYYY,
        ORDER_DD_MM_YYYY,
    };

private:
    readonly const AChar* MONS[] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
    };
    readonly const AChar* DAYS[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

public:
    TimeStamper(IN OPT EOrder order = orderDefault, IN OPT bool isDivideByNoon = isDivideByNoonDefault,
                IN OPT bool isMonthAsString = isMonthAsStringDefault, IN OPT bool isLeaveGap = isLeaveGapDefault);

public:
    std::string GetStamp() const;
    std::string GetDate() const;
    std::string GetTime() const;

public:
    void SetOrder(IN EOrder param);
    void SetMonthAsString(IN bool param);
    void SetDivideByNoon(IN bool param);
    void SetLeaveGap(IN bool param);

public:
    prop(put = SetOrder) EOrder Order;                 // Year, Month, Day order
    prop(put = SetMonthAsString) bool IsMonthAsString; // True: MAY         / False: 5 or 05
    prop(put = SetDivideByNoon) bool IsDivideByNoon;   // True: 1:00 PM     / False: 13:00
    prop(put = SetLeaveGap) bool IsLeaveGap;           // True: 1-1 1:00:00 / False: 01-01 01:00:00 << except min, sec

public:
    static void SetOrderDefault(IN EOrder param);
    static void SetMonthAsStringDefault(IN bool param);
    static void SetDivideByNoonDefault(IN bool param);
    static void SetLeaveGapDefault(IN bool param);
    static void SetDefaultState(IN EOrder order, IN bool divByNoon, IN bool monAsStr, IN bool leaveGap);

private:
    static void ReadSystemTime(OUT tm* param);
    static int  FractionalToMS(IN double sec);

public:
    static std::string SecToStamp(IN double time);
    static std::string SecToStamp24(IN double time, OUT OPT int* day = nullptr);

public:
    static std::string CreateStamp();
    static std::string CreateDate();
    static std::string CreateTime();

private:
    static std::string ToStringDate(IN tm* param, IN EOrder order, IN bool isMonthAsString, IN OPT bool isLeaveGap);
    static std::string ToStringTime(IN tm* param, IN bool isDivideByNoon, IN bool isLeaveGap);

private:
    EOrder order;
    bool   isMonthAsString; // True: MAY         / False: 5 or 05
    bool   isDivideByNoon;  // True: 1:00 PM     / False: 13:00
    bool   isLeaveGap;      // True: 1-1 1:00:00 / False: 01-01 01:00:00 << except min, sec

private:
    static EOrder orderDefault;
    static bool   isMonthAsStringDefault;
    static bool   isDivideByNoonDefault;
    static bool   isLeaveGapDefault;
};

} // namespace sbl
#endif