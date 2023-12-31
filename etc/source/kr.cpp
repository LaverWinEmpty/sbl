#include "kr.h"

// 각 to ㄱ / ㅏ / ㄱ
KR::Word KR::Table::Decompose(IN wchar_t ch)
{
    ch -= L'가';
    wchar_t initial = (wchar_t)((ch / INITIAL_CHANGE_UNIT) + L'ㄱ');
    wchar_t middle  = (wchar_t)(((ch % INITIAL_CHANGE_UNIT) / MIDDLE_CHANGE_UNIT) + L'ㅏ');
    wchar_t final   = (wchar_t)((ch % MIDDLE_CHANGE_UNIT) - 1); // [ㄱ: Value 1, Index 0] => EXCEPT (-1)

    if(final != EXCEPT) final += L'ㄱ';
    return Word{ initial, middle, final };
}

// USING CHAR => 각 from ㄱ / ㅏ / ㄱ
wchar_t KR::Table::Combine(IN wchar_t initial, IN wchar_t middle, IN wchar_t final)
{
    int i = GetInitialIndex(initial);
    int m = GetMiddleIndex(middle);
    int f = GetFinalIndex(final);

    return (wchar_t)(L'가' + i * INITIAL_CHANGE_UNIT + m * MIDDLE_CHANGE_UNIT + f); // [가 => 나]: 가 + 588
}

// Char Value To Index
int KR::Table::GetInitialIndex(IN wchar_t initial)
{
    initial -= L'ㄱ';
    if(initial < 0 || initial >= ARRAY_ELEMENTS_COUNT(INDEX_INITIAL)) {
        return 0;
    }
    return INDEX_INITIAL[initial];
}

// Char Value To Index
int KR::Table::GetMiddleIndex(IN wchar_t middle)
{
    middle -= L'ㅏ';
    if(middle < 0 || middle >= ARRAY_ELEMENTS_COUNT(INDEX_MIDDLE)) {
        return 0;
    }
    return INDEX_MIDDLE[middle];
}

// Char Value To Index
int KR::Table::GetFinalIndex(IN wchar_t final)
{
    if(final == L'\0') {
        return 0;
    }

    final = (wchar_t)(final - L'ㄱ' + 1);
    if(final < 0 || final >= ARRAY_ELEMENTS_COUNT(INDEX_FINAL)) {
        return 0;
    }
    return INDEX_FINAL[final]; // 0 = None, Begin: 1
}

// Index To Char
wchar_t KR::Table::GetInitialWord(IN int index)
{
    if(index < 0 || index >= ARRAY_ELEMENTS_COUNT(CHAR_LIST_INITIAL)) {
        return L'\0';
    }
    return CHAR_LIST_INITIAL[index];
}

// Index To Char
wchar_t KR::Table::GetMiddleWord(IN int index)
{
    if(index < 0 || index >= ARRAY_ELEMENTS_COUNT(CHAR_LIST_MIDDLE)) {
        return L'\0';
    }
    return CHAR_LIST_MIDDLE[index];
}

// Index To Char
wchar_t KR::Table::GetFinalWord(IN int index)
{
    if(index < 0 || index >= ARRAY_ELEMENTS_COUNT(CHAR_LIST_FINAL)) {
        return L'\0';
    }
    return CHAR_LIST_FINAL[index];
}

// EX) ㅡ + ㅣ = ㅢ, Failed: Return null
wchar_t KR::CombineMiddle(IN wchar_t a, IN wchar_t b)
{
    switch(a) {
        case L'ㅗ':
            switch(b) {
                case L'ㅏ': return L'ㅘ';
                case L'ㅐ': return L'ㅙ';
                case L'ㅣ': return L'ㅚ';
                default: return L'\0';
            }

        case L'ㅜ':
            switch(b) {
                case L'ㅓ': return L'ㅝ';
                case L'ㅔ': return L'ㅞ';
                case L'ㅣ': return L'ㅟ';
                default: return L'\0';
            }

        case L'ㅡ':
            if(b == L'ㅣ') return L'ㅢ';
            return L'\0';

        default: return L'\0';
    }
}

// EX) ㄱ + ㅅ = ㄳ, Failed: Return null
wchar_t KR::CombineFinal(IN wchar_t a, IN wchar_t b)
{
    switch(a) {
        case L'ㄱ':
            if(b == L'ㅅ') {
                return L'ㄳ';
            }
            return L'\0';

        case L'ㄴ':
            switch(b) {
                case L'ㅈ': return L'ㄵ';
                case L'ㅎ': return L'ㄶ';
                default: return L'\0';
            }

        case L'ㄹ':
            switch(b) {
                case L'ㄱ': return L'ㄺ';
                case L'ㅁ': return L'ㄻ';
                case L'ㅂ': return L'ㄼ';
                case L'ㅅ': return L'ㄽ';
                case L'ㅌ': return L'ㄾ';
                case L'ㅍ': return L'ㄿ';
                case L'ㅎ': return L'ㅀ';
                default: return L'\0';
            }

        case L'ㅂ':
            if(b == L'ㅅ') return L'ㅄ';
            return '\0';

        default: return '\0';
    }
}

// EX) ㄳ => ㄱ, Failed: Return param as it is
wchar_t KR::GetBeforeEnteredWord(IN wchar_t ch)
{
    switch(ch) {
            // Vowels
        case L'ㅘ': return L'ㅗ';
        case L'ㅙ': return L'ㅗ';
        case L'ㅚ': return L'ㅗ';
        case L'ㅝ': return L'ㅜ';
        case L'ㅞ': return L'ㅜ';
        case L'ㅟ': return L'ㅜ';
        case L'ㅢ':
            return L'ㅡ';
            // consona
        case L'ㄳ': return L'ㄱ';
        case L'ㄵ': return L'ㄴ';
        case L'ㄶ': return L'ㄴ';
        case L'ㄺ': return L'ㄹ';
        case L'ㄻ': return L'ㄹ';
        case L'ㄽ': return L'ㄹ';
        case L'ㄾ': return L'ㄹ';
        case L'ㄿ': return L'ㄹ';
        case L'ㅀ': return L'ㄹ';
        case L'ㅄ':
            return L'ㅂ';
            // default
        default: return ch;
    }
}

// EX) ㄳ => return ㅅ, Failed: Return null
wchar_t KR::GetAfterEnteredWord(IN wchar_t ch)
{
    switch(ch) {
            // Vowels
        case L'ㅘ': return L'ㅏ';
        case L'ㅙ': return L'ㅐ';
        case L'ㅚ': return L'ㅣ';
        case L'ㅝ': return L'ㅓ';
        case L'ㅞ': return L'ㅔ';
        case L'ㅟ': return L'ㅣ';
        case L'ㅢ':
            return L'ㅣ';
            // consonants
        case L'ㄳ': return L'ㅅ';
        case L'ㄵ': return L'ㅈ';
        case L'ㄶ': return L'ㅎ';
        case L'ㄺ': return L'ㄱ';
        case L'ㄻ': return L'ㅁ';
        case L'ㄽ': return L'ㅅ';
        case L'ㄾ': return L'ㅌ';
        case L'ㄿ': return L'ㅍ';
        case L'ㅀ': return L'ㅗ';
        case L'ㅄ':
            return L'ㅅ';
            // default
        default: return '\0';
    }
}

// Return: Before entered (ㅢ => ㅡ)
// Out(Optional): After entered (ㅢ => ㅣ)
// Failed: Return param as it is
wchar_t KR::Decompose(IN wchar_t ch, OUT OPT wchar_t* after)
{
    if(after != nullptr) {
        *after = GetAfterEnteredWord(IN ch); // Not mixed
        // Failed
        if(*after == L'\0') {
            return ch;
        }
    }

    return GetBeforeEnteredWord(ch);
}

// EX) ㄱ,ㅏ ... => true, 가, A ... => false
bool KR::IsDecomposed(IN wchar_t ch)
{
    if(ch < L'ㄱ' || ch > L'ㅣ') {
        return false;
    }
    return true;
}

KR::EKinds KR::GetKinds(IN wchar_t ch)
{
    if(true == IsDecomposed(ch)) {
        if(ch <= L'ㅎ') {
            return EKinds::CONSONANTS;
        }
        if(ch >= L'ㅏ') {
            return EKinds::VOWELS;
        }
    }
    if(ch >= L'가' || ch <= L'힣') return EKinds::COMPLITION;
    return EKinds::NOT_KOR;
}

/*
    KR::Buffer
*/

// EX) [ㅏ + ㅏ]
wchar_t KR::Buffer::VowelsMixFailed(IN wchar_t input)
{
    wchar_t retval;
    switch(_cursor) {
            // EX) [ㅏ => ㅓ]: keep ㅓ, retrun ㅏ
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [ㄱ => ㅏ => ㅓ]: keep ㅓ, return 가
        case 1:
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1]); // EX) ㄱ + ㅏ
            _buffer._array[0] = input;                                                // EX) ㅓ
            _cursor           = 0;                                                    // Next Word
            return retval;

            // case 2: { ㄱ, ㅏ, ㅏ }: Impossible
        default: return L'\0';
    }
}

// EX) [ㄱ => ㅏ]
wchar_t KR::Buffer::InputVowelAfterConsonant(IN wchar_t input)
{
    wchar_t lastInput = _buffer._array[_cursor];
    wchar_t retval    = L'\0';

    switch(_cursor) {
        case 0:
            // EX) [ㄳ => ㅏ]: Keep 사, return ㄱ
            if(EXCEPT == Table::GetInitialIndex(lastInput)) {
                wchar_t after, before = Decompose(_buffer._array[0], &after);

                retval            = before; // EX) ㄱ
                _buffer._array[0] = after;  // EX) ㅅ
                _buffer._array[1] = input;  // EX) ㅏ
                _cursor           = 1;      // Next word
                return retval;
            }

            // Else
            // EX) [ㄱ => ㅏ]: Keep 가
            ++_cursor; // Add input word
            _buffer._array[_cursor] = input;
            return retval;

        case 2:
            // Ex) [ㄱ => ㅏ => ㄱ => ㅅ => ㅏ]: Keep 사, return 각
            if(EXCEPT == Table::GetInitialIndex(lastInput)) {
                wchar_t after, before = Decompose(lastInput, &after);

                retval            = Table::Combine(_buffer._array[0], _buffer._array[1], before); // EX) 각
                _buffer._array[0] = after;                                                        // EX) ㅅ
                _buffer._array[1] = input;                                                        // EX) ㅏ
                _cursor           = 1;                                                            // Next word
                return retval;
            }

            // Else
            // EX) [ㄱ => ㅏ => ㄴ => ㅏ]: Keep 나, return 가
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1]); // EX) 가
            _buffer._array[0] = _buffer._array[2];                                    // EX) ㄴ
            _buffer._array[1] = input;                                                // EX) ㅏ
            _cursor           = 1;                                                    // Next Word
            return retval;

            // case 1: { ㅏ, ㄱ, ㅏ + input }: Impossible
        default: return L'\0'; // Unused
    }
}

// EX) [ㄱ => ㄴ]
wchar_t KR::Buffer::ConsonantsMixFailed(IN wchar_t input)
{
    wchar_t retval;

    switch(_cursor) {
            // EX) [ㄱ => ㄴ]: Keep ㄴ, return ㄱ
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [ㄱ => ㅏ => ㄱ => ㅅ => ㄴ]: Keep ㄴ, return 갃
        case 2:
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1], _buffer._array[2]);
            _buffer._array[0] = input; // Next
            _cursor           = 0;     // Next word
            return retval;

            // case 1: { ㄱ, ㄴ + input }: Impossible
        default: return L'\0'; // Unused
    }
}

// EX) [ㅏ => ㄱ]
wchar_t KR::Buffer::InputConsonantsAfterVowels(IN wchar_t input)
{
    wchar_t retval;
    switch(_cursor) {
            // EX) [ㅏ => ㄱ]: Keep ㄱ, return ㅏ
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [ㄱ =>  (ㅗ => ㅏ) => ㄱ]: Keep 곽
        case 1:
            ++_cursor; // Add input word
            _buffer._array[_cursor] = input;
            return L'\0';

            // case 2: { ㅏ, ㄱ, ㅏ + ㄱ }: Impossible
        default: return L'\0'; // Unused
    }
}

// Return not null: Input complition
wchar_t KR::Buffer::Enter(IN wchar_t input)
{
    // First
    if(true == IsEmpty()) {
        _buffer._array[_cursor] = input;
        return L'\0';
    }

    wchar_t lastInput = _buffer._array[_cursor];
    EKinds  curr      = GetKinds(input);
    EKinds  last      = GetKinds(lastInput);

    // is Vowel
    if(curr == EKinds::VOWELS) {
        if(last == EKinds::VOWELS) // Same
        {
            wchar_t mixed = CombineMiddle(lastInput, input); // EX) ㅡ +ㅣ = ㅢ
            if(mixed == 0)                                   // Failed, EX) ㅏ + ㅏ = Null
                return VowelsMixFailed(input);

            // Else: Mix succeed
            _buffer._array[_cursor] = mixed; // Reallocation
            return L'\0';
        }

        // Else: Different kind
        return InputVowelAfterConsonant(input);
    }

    // Else: CONSONANT:
    if(last == EKinds::CONSONANTS) // Same
    {
        wchar_t mixed = CombineFinal(lastInput, input); // EX) ㄱ + ㅅ = ㄳ
        if(mixed == 0)                                  // Failed, EX)  ㄱ + ㄴ = Null
            return ConsonantsMixFailed(input);

        // Else: Mix succeed
        _buffer._array[_cursor] = mixed;
        return L'\0';
    }

    // Else: Different kind
    return InputConsonantsAfterVowels(input);
}

// Remove last input
// Return: True => Removed, False => Not
bool KR::Buffer::Remove()
{
    wchar_t last = _buffer._array[_cursor];

    // Empty
    if(last == L'\0') {
        return false;
    }

    // EX) ㄳ => ㄱ OR ㅢ => ㅡ
    wchar_t removed = GetBeforeEnteredWord(last);

    // Not decompose => Remove (EX) ㄱ => ㄱ)
    if(removed == last) {
        _buffer._array[_cursor] = L'\0';
        if(_cursor > 0) --_cursor;
        return true;
    }

    // Else
    _buffer._array[_cursor] = removed;
    return true;
}

// Getter(currently created word)
wchar_t KR::Buffer::GetWord()
{
    switch(_cursor) {
        case 0: return _buffer._array[0];
        case 1: return Table::Combine(_buffer._array[0], _buffer._array[1]);
        case 2:
            return Table::Combine(_buffer._array[0], _buffer._array[1], _buffer._array[2]);
            // Default: Impossible
        default: return L'\0';
    }
}

// Get + Init
wchar_t KR::Buffer::EndInput()
{
    wchar_t word = GetWord();
    Clear();
    return word;
}

// Return not null: Input complition (Enter + Exception handling)
int KR::Buffer::TryInput(IN wchar_t kr)
{
    if(false == IsDecomposed(kr)) {
        return EXCEPT;
    }
    return Enter(kr);
}

void KR::Append(wchar_t input)
{
    int res = _stream.TryInput(input);

    // Other words
    if(res == EXCEPT) {
        wchar_t last = _stream.EndInput(); // End
        _written += last;                  // append
        _written += input;                 // append
        return;
    }

    // Not complition
    if(res == L'\0') return;

    // Complition
    else _written += (wchar_t)res; // append
}

void KR::Remove()
{
    if(true == _stream.IsEmpty()) {
        _written.pop_back();
    }

    else {
        _stream.Remove();
    }
}

void KR::Clear()
{
    _stream.Clear();
    _written.clear();
}

std::wstring KR::GetWritten()
{
    // Written + Current buffer
    std::wstring temp = _written;
    return temp + _stream.GetWord();
}