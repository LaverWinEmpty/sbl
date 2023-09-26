#ifndef SBL_CONSTANTS_HEADER__
#define SBL_CONSTANTS_HEADER__

#include "defs.h"
#include "types.h"

namespace sbl {

struct EErrorMessages
{
    readonly const AChar* NULL_POINTER  = "Null Pointer Exception.";
    readonly const AChar* NEW_FAILED    = "Memory Allocation Failed.";
    readonly const AChar* CREATE_FAILED = "Object Creation Failed";
    readonly const AChar* NOT_FOUND     = "Resource Not Found.";
    readonly const AChar* INVALID_VALUE = "Unprocessable.";
    readonly const AChar* OUT_OF_RANGE  = "Index Out of Range.";
    readonly const AChar* FORMAT_ERROR  = "Data Structure Mismatch.";
    readonly const AChar* ACCESS_FAILED = "Aceess Denied.";
};

// Simple result code
enum class EResult
{
    EXCEPTED = -1, // Unknown error
    FAILED   = 0,  // User error
    SUCCEED  = 1,
};

// <, ==, > to value
enum class EComparison
{
    LESS  = -1,
    EQUAL = 0,
    MORE  = 1,
};

} // namespace sbl
#endif
