#include "../include/timeStamper.h"

namespace sbl {

TimeStamper::EOrder TimeStamper::orderDefault           = TimeStamper::EOrder::ORDER_YYYY_MM_DD;
bool                TimeStamper::isMonthAsStringDefault = false;
bool                TimeStamper::isDivideByNoonDefault  = false;
bool                TimeStamper::isLeaveGapDefault      = false;

TimeStamper::TimeStamper(IN OPT EOrder order, IN OPT bool isMonthAsString, IN OPT bool isDivideByNoon,
                         IN OPT bool isLeaveGap):
    isDivideByNoon(isDivideByNoon),
    isMonthAsString(isMonthAsString), order(order), isLeaveGap(isLeaveGap)
{}

// Date + Time
std::string TimeStamper::GetStamp() const
{
    tm temp;
    ReadSystemTime(&temp);
    std::string result = ToStringDate(&temp, order, isMonthAsString, isLeaveGap);
    return result + ' ' + ToStringTime(&temp, isDivideByNoon, isLeaveGap);
}

// YYYY-MM-DD
std::string TimeStamper::GetDate() const
{
    tm temp;
    ReadSystemTime(&temp);
    return ToStringDate(&temp, order, isMonthAsString, isLeaveGap);
}

// 00:00:00
std::string TimeStamper::GetTime() const
{
    tm temp;
    ReadSystemTime(&temp);
    return ToStringTime(&temp, isDivideByNoon, isLeaveGap);
}

void TimeStamper::SetOrder(IN EOrder param)
{
    order = param;
}

void TimeStamper::SetMonthAsString(IN bool param)
{
    isMonthAsString = param;
}

void TimeStamper::SetDivideByNoon(IN bool param)
{
    isDivideByNoon = param;
}

void TimeStamper::SetLeaveGap(IN bool param)
{
    isLeaveGap = param;
}

void TimeStamper::SetOrderDefault(IN EOrder param)
{
    orderDefault = param;
}

void TimeStamper::SetMonthAsStringDefault(IN bool param)
{
    isMonthAsStringDefault = param;
}

void TimeStamper::SetDivideByNoonDefault(IN bool param)
{
    isDivideByNoonDefault = param;
}

void TimeStamper::SetLeaveGapDefault(IN bool param)
{
    isLeaveGapDefault = param;
}

void TimeStamper::SetDefaultState(IN EOrder order, IN bool divByNoon, IN bool monAsStr, IN bool leaveGap)
{
    orderDefault           = order;
    isDivideByNoonDefault  = divByNoon;
    isMonthAsStringDefault = monAsStr;
    isLeaveGapDefault      = leaveGap;
}

// Init tm struct
void TimeStamper::ReadSystemTime(OUT tm* param)
{
    time_t curr;
    time(&curr);
    localtime_s(param, &curr);
}

// EX) 123.123 => 0.123 => 12.3 => 12
int TimeStamper::FractionalToMS(IN double time)
{
    return static_cast<int>((time - floor(time)) * 100);
}

// "Decimal part => sec, Fractional part => ms"
// EX) 1.1 => 01'01
// Max: (1e4086 - 1):59:59.99
// 4086 == default buffer size - 10 (value + ":00:00.00" + null)
std::string TimeStamper::SecToStamp(IN double time)
{
    // ABS:: To calculate the date
    if(time < 0) {
        time *= -1;
    }

    UInt64   nTime = static_cast<UInt64>(time);
    UInt64   hour  = static_cast<int>(nTime / 3600);
    unsigned min   = (nTime / 60) % 60;
    unsigned sec   = nTime % 60;
    unsigned ms    = FractionalToMS(time);

    // "00:00'00", 9 byte
    char buffer[DEF_BUF_SIZE];

    if(hour != 0) {
        sprintf_s(buffer, "%02lld:%02d:%02d.%02d", hour, min, sec, ms);
    }

    else if(min != 0) {
        sprintf_s(buffer, "%02d:%02d.%02d", min, sec, ms);
    }

    else {
        sprintf_s(buffer, "%02d.%02d", sec, ms);
    }

    return buffer;
}

// "Decimal part => sec, Fractional part => ms"
// EX) 1.1 => 00:00:01'01
//
// # About max #
//
// 23:59:59'99, 364 day (== 31535999)
// integer: 31535999     => 31535999 << Max
// float:   31535999.99  => 31536000
//          31535999.00  =? 31535998 << Max
// double:  31535999.99  => 31535998
//          31539999.991 => 31535999 << Max
std::string TimeStamper::SecToStamp24(IN double time, OUT OPT int* day)
{
    // ABS:: To calculate the date
    if(time < 0) {
        time *= -1;
    }

    tm     temp;
    time_t sec = static_cast<time_t>(time);
    int    ms  = FractionalToMS(time);
    gmtime_s(&temp, &sec);

    // "00:00:00'00", 12 byte
    char buffer[12];
    sprintf_s(buffer, "%02d:%02d:%02d.%02d", temp.tm_hour, temp.tm_min, temp.tm_sec, ms);
    if(day) {
        *day = temp.tm_yday;
    }

    return buffer;
}

// Get Date + Time (static)
std::string TimeStamper::CreateStamp()
{
    tm temp;
    ReadSystemTime(&temp);
    std::string result = ToStringDate(&temp, orderDefault, isMonthAsStringDefault, isLeaveGapDefault);
    return result + ' ' + ToStringTime(&temp, isDivideByNoonDefault, isLeaveGapDefault);
}

// Get Date (static)
std::string TimeStamper::CreateDate()
{
    tm temp;
    ReadSystemTime(&temp);
    return ToStringDate(&temp, orderDefault, isMonthAsStringDefault, isLeaveGapDefault);
}

// Get Time (static)
std::string TimeStamper::CreateTime()
{
    tm temp;
    ReadSystemTime(&temp);
    return ToStringTime(&temp, isDivideByNoonDefault, isLeaveGapDefault);
}

std::string TimeStamper::ToStringDate(IN tm* param, IN EOrder order, IN bool isMonthAsString, IN bool isLeaveGap)
{
    // "1900_MAY_00" <= 12
    AChar buffer[12] = { 0 };

    int   year   = param->tm_year + 1900;
    AChar day[4] = { 0 };
    AChar mon[4] = { 0 };
    AChar delimiter;

    if(isMonthAsString) {
        sprintf_s(mon, "%s", MONS[param->tm_mon]);
        delimiter = ' ';
    }

    else {
        int nMon = param->tm_mon + 1;
        if(isLeaveGap) {
            sprintf_s(mon, "%d", nMon);
        }
        else {
            sprintf_s(mon, "%02d", nMon);
        }
        delimiter = '-';
    }

    if(isLeaveGap) {
        sprintf_s(day, "%d", param->tm_mday);
    }
    else {
        sprintf_s(day, "%02d", param->tm_mday);
    }

    switch(order) {
        case EOrder::ORDER_YYYY_MM_DD: sprintf_s(buffer, "%d%c%s%c%s", year, delimiter, mon, delimiter, day); break;
        case EOrder::ORDER_MM_DD_YYYY: sprintf_s(buffer, "%s%c%s%c%d", mon, delimiter, day, delimiter, year); break;
        case EOrder::ORDER_DD_MM_YYYY: sprintf_s(buffer, "%s%c%s%c%d", day, delimiter, mon, delimiter, year); break;
    }

    return buffer;
}

std::string TimeStamper::ToStringTime(IN tm* param, IN bool isDivideByNoon, IN bool isLeaveGap)
{
    // "00:00:00 AM" <= 12
    AChar buffer[12] = { 0 };

    int          hour = param->tm_hour;
    const AChar* noon = "AM";

    if(isDivideByNoon) {
        // Check AM / PM
        if(hour >= 12) {
            noon = "PM";
        }

        // Divide AM / PM
        if(hour == 0) {
            hour = 24;
        }

        if(hour > 12) {
            hour -= 12;
        }
    }

    std::string format;
    if(isLeaveGap) {
        format = "%d:%02d:%02d";
    }
    else {
        format = "%02d:%02d:%02d";
    }

    if(isDivideByNoon) {
        sprintf_s(buffer, (format + " %s").c_str(), hour, param->tm_min, param->tm_sec, noon);
    }
    else {
        sprintf_s(buffer, format.c_str(), hour, param->tm_min, param->tm_sec);
    }

    return buffer;
}

} // namespace sbl