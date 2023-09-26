#ifndef SBL_ERROR_HEADER__
#define SBL_ERROR_HEADER__

#include "symbols.h"

namespace sbl {

class ErrorBuilder;

template<typename T, typename U> struct Error
{
    Error(IN const AChar* message, IN const T& inactual, IN const U& except):
        message(message), actual(actual), except(except)
    {}
    const AChar* message;
    T            actual;
    U            except;

private:
    friend class ErrorBuilder;
};

// Value only
template<typename T> struct Error<T, void>
{
    Error(IN const AChar* message, IN const T& actual): message(message), actual(actual) {}
    const AChar* message;
    T            actual;

private:
    friend class ErrorBuilder;
};
template<typename T> using ErrVal = Error<T, void>;

// Message only
template<> struct Error<void, void>
{
    Error(IN const AChar* message): message(message) {}
    const AChar* message;

private:
    friend class ErrorBuilder;
};
using ErrMsg = Error<void, void>;

class ErrorBuilder
{
public:
    DECLARE_LIMIT_LIFECYCLE(ErrorBuilder);

public:
    static ErrMsg NullException();
    static ErrMsg NewFailed();
    static ErrMsg CreateFailed();
    static ErrMsg NotFound();
    static ErrMsg InvalidValue();
    static ErrMsg OutOfRange();
    static ErrMsg FormatError();
    static ErrMsg AccessFailed();

public:
    template<typename T> static ErrVal<T> NullException(IN T actual);
    template<typename T> static ErrVal<T> NewFailed(IN T actual);
    template<typename T> static ErrVal<T> CreateFailed(IN T actual);
    template<typename T> static ErrVal<T> ReturnNull(IN T actual);
    template<typename T> static ErrVal<T> NotFound(IN T actual);
    template<typename T> static ErrVal<T> InvalidValue(IN T actual);
    template<typename T> static ErrVal<T> OutOfRange(IN T actual);
    template<typename T> static ErrVal<T> FormatError(IN T actual);
    template<typename T> static ErrVal<T> AccessFailed(IN T actual);

public:
    template<typename T, typename U> static Error<T, U> NullException(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> NewFailed(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> CreateFailed(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> NotFound(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> InvalidValue(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> OutOfRange(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> FormatError(IN T actual, IN U except);
    template<typename T, typename U> static Error<T, U> AccessFailed(IN T actual, IN U except);
};

inline ErrMsg ErrorBuilder::NullException()
{
    return Error<void, void>{ EErrorMessages::NULL_POINTER };
}

inline ErrMsg ErrorBuilder::NewFailed()
{
    return Error<void, void>{ EErrorMessages::NEW_FAILED };
}

inline ErrMsg ErrorBuilder::CreateFailed()
{
    return Error<void, void>{ EErrorMessages::CREATE_FAILED };
}

inline ErrMsg ErrorBuilder::NotFound()
{
    return Error<void, void>{ EErrorMessages::NOT_FOUND };
}

inline ErrMsg ErrorBuilder::InvalidValue()
{
    return Error<void, void>{ EErrorMessages::INVALID_VALUE };
}

inline ErrMsg ErrorBuilder::OutOfRange()
{
    return Error<void, void>{ EErrorMessages::OUT_OF_RANGE };
}

inline ErrMsg ErrorBuilder::FormatError()
{
    return Error<void, void>{ EErrorMessages::FORMAT_ERROR };
}

inline ErrMsg ErrorBuilder::AccessFailed()
{
    return Error<void, void>{ EErrorMessages::ACCESS_FAILED };
}

template<typename T> inline ErrVal<T> ErrorBuilder::NullException(IN T actual)
{
    return Error<T, void>{ EErrorMessages::NULL_POINTER, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::NewFailed(IN T actual)
{
    return Error<T, void>{ EErrorMessages::NEW_FAILED, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::CreateFailed(IN T actual)
{
    return Error<T, void>{ EErrorMessages::CREATE_FAILED, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::NotFound(IN T actual)
{
    return Error<T, void>{ EErrorMessages::NOT_FOUND, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::InvalidValue(IN T actual)
{
    return Error<T, void>{ EErrorMessages::INVALID_VALUE, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::OutOfRange(IN T actual)
{
    return Error<T, void>{ EErrorMessages::OUT_OF_RANGE, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::FormatError(IN T actual)
{
    return Error<T, void>{ EErrorMessages::FORMAT_ERROR, actual };
}

template<typename T> inline ErrVal<T> ErrorBuilder::AccessFailed(IN T actual)
{
    return Error<T, void>{ EErrorMessages::ACCESS_FAILED, actual };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::NullException(IN T actual, IN U except)
{
    return Error{ EErrorMessages::NULL_POINTER, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::NewFailed(IN T actual, IN U except)
{
    return Error{ EErrorMessages::NEW_FAILED, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::CreateFailed(IN T actual, IN U except)
{
    return Error{ EErrorMessages::CREATE_FAILED, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::NotFound(IN T actual, IN U except)
{
    return Error{ EErrorMessages::NOT_FOUND, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::InvalidValue(IN T actual, IN U except)
{
    return Error{ EErrorMessages::INVALID_VALUE, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::OutOfRange(IN T actual, IN U except)
{
    return Error{ EErrorMessages::OUT_OF_RANGE, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::FormatError(IN T actual, IN U except)
{
    return Error{ EErrorMessages::FORMAT_ERROR, actual, except };
}

template<typename T, typename U> inline Error<T, IN U> ErrorBuilder::AccessFailed(IN T actual, IN U except)
{
    return Error{ EErrorMessages::ACCESS_FAILED, actual, except };
}

} // namespace sbl
#endif