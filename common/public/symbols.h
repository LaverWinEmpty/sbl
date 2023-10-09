#ifndef SBL_CONSTANTS_HEADER__
#define SBL_CONSTANTS_HEADER__

#include "defs.h"
#include "types.h"

namespace sbl {

struct EErrMsg
{
    readonly const char* UNKNOWN       = "ERROR MESSAGE LOADING FAILED";
    readonly const char* NULL_POINTER  = "NULL POINTER EXCEPTION.";
    readonly const char* NEW_FAILED    = "MEMORY ALLOCATION FAILED.";
    readonly const char* CREATE_FAILED = "OBJECT CREATION FAILED";
    readonly const char* NOT_FOUND     = "RESOURCE NOT FOUND.";
    readonly const char* INVALID_VALUE = "UNPROCESSABLE.";
    readonly const char* OUT_OF_RANGE  = "INDEX OUT OF RANGE.";
    readonly const char* FORMAT_ERROR  = "DATA STRUCTURE MISMATCH.";
    readonly const char* ACCESS_FAILED = "ACEESS DENIED.";
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
