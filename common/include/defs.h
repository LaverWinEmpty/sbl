#ifndef __SEMIBASE_MACROS_HEADER__
#define __SEMIBASE_MACROS_HEADER__

/*
    2023.09.24
    Write: Windows OS / ANSI (CP-949)
*/

// Check OS
#if _WIN32 || _WIN64
#    if _WIN64
#        define X64
#    else
#        define X86
#    endif
#elif __GNUC__
#    if __x86_64__ || __ppc64__
#        define X64
#    else
#        define X86
#    endif
#else
#    error Unknown OS
#endif

// Check Endian
#if defined(__BYTE_ORDER__)
#    if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#        define BIG_ENDIAN
#    elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#        define LITTLE_ENDIAN
#    else
#        error Unknown Endian
#    endif
#endif

#ifndef interface
#    define interface struct
#endif

#ifndef readonly
#    define readonly static constexpr
#endif

#ifndef typeof
#    define typeof(t) decltype(t)
#endif

// MSVC / Clang
// property, __VA_ARGS__: get / put
#if _WIN32 || _WIN64
#    ifndef prop
#        define prop(...) __declspec(property(__VA_ARGS__))
#    endif
#endif

#ifndef IN
#    define IN
#endif

#ifndef OUT
#    define OUT
#endif

#ifndef OPT
#    define OPT
#endif

#ifndef NULL
#    define NULL 0
#endif

#ifndef EXCEPT
#    define EXCEPT -1
#endif

#ifndef BYTE_BIT
#    define BYTE_BIT 8
#endif

#ifndef DEF_BUF_SIZE
#    define DEF_BUF_SIZE 4096 // 4kb
#endif

#ifndef EPSILON
#    define EPSILON 1.192092896e-07f // float
#endif

#ifndef PI
#    define PI 3.14159265358979f // double
#endif

// To string
#define TO_STRING(x) #x

// Macro value to string
#define STR_MACRO(x) TO_STRING(x)

// Simple concatenate
#define STR_CONCAT(x, y) TO_STRING(x)##y

// dll
#define EXPORT extern "C" __declspec(dllexport)

// Check big endian
#define CHECK_BIG_ENDIAN() (static_cast<bool>(*("\0\1")))

// Check little endian
#define CHECK_LITTLE_ENDIAN() (static_cast<bool>(*("\1\0")))

// Casting: Const type to normal type
#define RAW_CAST(from, to, ptr) (reinterpret_cast<to>(const_cast<from>((ptr))))

// Casting: To int
#define TO_INT(x) (static_cast<int>(x))

// Ptr to MAR size integer
#define PTR_TO_WORD(x) (reinterpret_cast<WORD_PTR>((x)))

// Ptr To int
#define PTR_TO_INT(x) (TO_INT(PTR_TO_WORD((x))))

// Get absolute value
#define ABS(x) ((x) < 0 ? -(x) : (x))

// Compare float / double
#define FLOAT_EQUAL(a, b) (ABS((a) - (b)) <= EPSILON)

// Bool to astring
#define BOOL_ASTR(x) ((x) ? "TRUE" : "FALSE")

// Bool to wstring
#define BOOL_WSTR(x) ((x) ? L"TRUE" : L"FALSE")

// Get maxmum value
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Get minimum value
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// Radian to degree
#define RADIAN_TO_DEGREE(radian) (radian * (180 / PI))

// Degree to radian
#define DEGREE_TO_RADIAN(degree) (degree * (PI / 180))

// ASCII to Upper
#define TO_UPPER(ch) ((ch)-32)

// ASCII to Lower
#define TO_LOWER(ch) ((ch) | 0x20)

// Bit To Byte
#define TO_BYTE(x) (x >> 3)

// Byte to Bit
#define TO_BIT(x) (x << 3)

// sizeof to Bit
#define SIZE_BIT(type) (sizeof(type) << 3)

// Get array element amount, NOT POINTER
#define ARRAY_ELEMENTS_COUNT(arr) (sizeof((arr)) / sizeof(*(arr)))

// Rotate: UNSIGNED OLNY
#define SHIFT_ROT_L_UNSIGNED(x, move) (((x) << (move)) | ((x) >> (SIZE_BIT((x)) - (move))))

// Rotate: UNSIGNED OLNY
#define SHIFT_ROT_R_UNSIGNED(x, move) (((x) >> (move)) | ((x) << (SIZE_BIT((x)) - (move))))

// For safe shift: Removes extended filled 1-bit.
#define TO_INT64(x)                                                                                                    \
    (static_cast<unsigned __int64>(x) << ((sizeof(__int64) - sizeof((x))) << 3) >>                                     \
     ((sizeof(unsigned __int64) - sizeof((x))) << 3))

// Shift Rotate to Left => Signed safe
#define SHIFT_ROT_L(x, move) (((TO_INT64(x)) << (move)) | ((TO_INT64(x)) >> (SIZE_BIT((x)) - (move))))

// Shift Rotate to Right => Signed safe
#define SHIFT_ROT_R(x, move) (((TO_INT64(x)) >> (move)) | ((TO_INT64(x)) << (SIZE_BIT((x)) - (move))))

// 0x0011 => 0x1100
#define REVERSE_ENDIAN_16(x) (((x) << 8) | (((x)&0xFF00) >> 8))

// 0x00112233 => 0x33221100
#define REVERSE_ENDIAN_32(x) (((x) << 24) | (((x)&0xFF00) << 8) | (((x)&0xFF0000) >> 8) | (((x)&0xFF000000) >> 24))

// 0x0011223344556677 => 0x7766554433221100
#define REVERSE_ENDIAN_64(x)                                                                                           \
    (((x) << 56) | (((x)&0xFF00) << 40) | (((x)&0xFF0000) << 24) | (((x)&0xFF000000) << 8) |                           \
     (((x)&0xFF00000000) >> 8) | (((x)&0xFF0000000000) >> 24) | (((x)&0xFF000000000000) >> 40) |                       \
     (((x)&0xFF00000000000000) >> 56))

// Duff's device
#define FAST_LOOP(loop, procedure)                                                                                     \
    do {                                                                                                               \
        __int64 loop_count_in_fast_loop_macro = (static_cast<__int64>(loop) + 7) >> 3;                                 \
        if(loop > 0) switch(loop & 0b111) {                                                                            \
                case 0: do { procedure;                                                                                \
                        case 7: procedure;                                                                             \
                        case 6: procedure;                                                                             \
                        case 5: procedure;                                                                             \
                        case 4: procedure;                                                                             \
                        case 3: procedure;                                                                             \
                        case 2: procedure;                                                                             \
                        case 1: procedure;                                                                             \
                    } while(--loop_count_in_fast_loop_macro > 0);                                                      \
            }                                                                                                          \
    } while(false)

// Swap
#define SWAP(a, b)                                                                                                     \
    do {                                                                                                               \
        typeof(a) temp_in_swap_macro = (a);                                                                            \
        (a)                          = (b);                                                                            \
        (b)                          = temp_in_swap_macro;                                                             \
    } while(false)

// Delete, Set nullptr
#define SAFE_DELETE(ptr)                                                                                               \
    do {                                                                                                               \
        if(ptr != NULL) delete ptr;                                                                                    \
        ptr = nullptr;                                                                                                 \
    } while(false)

// Delete ARRAY, Set nullptr
#define SAFE_DELETES(ptr)                                                                                              \
    do {                                                                                                               \
        if(ptr != NULL) delete[] ptr;                                                                                  \
        ptr = nullptr;                                                                                                 \
    } while(false)

// Delete, Not set nullptr
#define SAFE_DESTRUCT(ptr)                                                                                             \
    do {                                                                                                               \
        if(ptr != NULL) delete ptr;                                                                                    \
    } while(false)

// Delete ARRAY, Not set nullptr
#define SAFE_DESTRUCTS(ptr)                                                                                            \
    do {                                                                                                               \
        if(ptr != NULL) delete[] ptr;                                                                                  \
    } while(false)

// Set Singleton
#define DECLARE_SINGLETON(Type)                                                                                        \
    static void CreateInstance()                                                                                       \
    {                                                                                                                  \
        if(instance == nullptr) {                                                                                      \
            instance = new Type();                                                                                     \
        }                                                                                                              \
    }                                                                                                                  \
    static void DestroyInstance()                                                                                      \
    {                                                                                                                  \
        if(instance != nullptr) {                                                                                      \
            delete instance;                                                                                           \
        }                                                                                                              \
    }                                                                                                                  \
    static Type* GetInstance()                                                                                         \
    {                                                                                                                  \
        return instance;                                                                                               \
    }

// Instance
#define DECLARE_INSTANCE(Type) static Type* instance

// Initialize static variable
#define DEFINE_INSTANCE(Type) Type* Type::instance = nullptr

// Get Singleton
#define INSTANCE(Type) Type::GetInstance()

// Set utility class
#define DECLARE_LIMIT_LIFECYCLE(Type)                                                                                  \
    Type()  = delete;                                                                                                  \
    ~Type() = delete

// No copy
#define DECLARE_NO_COPY(Type)                                                                                          \
    Type(const Type&)            = delete;                                                                             \
    Type& operator=(const Type&) = delete

#endif