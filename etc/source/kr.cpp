#include "kr.h"

// °¢ to ¤¡ / ¤¿ / ¤¡
KR::Word KR::Table::Decompose(IN wchar_t ch)
{
    ch -= L'°¡';
    wchar_t initial = (wchar_t)((ch / INITIAL_CHANGE_UNIT) + L'¤¡');
    wchar_t middle  = (wchar_t)(((ch % INITIAL_CHANGE_UNIT) / MIDDLE_CHANGE_UNIT) + L'¤¿');
    wchar_t final   = (wchar_t)((ch % MIDDLE_CHANGE_UNIT) - 1); // [¤¡: Value 1, Index 0] => EXCEPT (-1)

    if(final != EXCEPT) final += L'¤¡';
    return Word{ initial, middle, final };
}

// USING CHAR => °¢ from ¤¡ / ¤¿ / ¤¡
wchar_t KR::Table::Combine(IN wchar_t initial, IN wchar_t middle, IN wchar_t final)
{
    int i = GetInitialIndex(initial);
    int m = GetMiddleIndex(middle);
    int f = GetFinalIndex(final);

    return (wchar_t)(L'°¡' + i * INITIAL_CHANGE_UNIT + m * MIDDLE_CHANGE_UNIT + f); // [°¡ => ³ª]: °¡ + 588
}

// Char Value To Index
int KR::Table::GetInitialIndex(IN wchar_t initial)
{
    initial -= L'¤¡';
    if(initial < 0 || initial >= ARRAY_ELEMENTS_COUNT(INDEX_INITIAL)) {
        return 0;
    }
    return INDEX_INITIAL[initial];
}

// Char Value To Index
int KR::Table::GetMiddleIndex(IN wchar_t middle)
{
    middle -= L'¤¿';
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

    final = (wchar_t)(final - L'¤¡' + 1);
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

// EX) ¤Ñ + ¤Ó = ¤Ò, Failed: Return null
wchar_t KR::CombineMiddle(IN wchar_t a, IN wchar_t b)
{
    switch(a) {
        case L'¤Ç':
            switch(b) {
                case L'¤¿': return L'¤È';
                case L'¤À': return L'¤É';
                case L'¤Ó': return L'¤Ê';
                default: return L'\0';
            }

        case L'¤Ì':
            switch(b) {
                case L'¤Ã': return L'¤Í';
                case L'¤Ä': return L'¤Î';
                case L'¤Ó': return L'¤Ï';
                default: return L'\0';
            }

        case L'¤Ñ':
            if(b == L'¤Ó') return L'¤Ò';
            return L'\0';

        default: return L'\0';
    }
}

// EX) ¤¡ + ¤µ = ¤£, Failed: Return null
wchar_t KR::CombineFinal(IN wchar_t a, IN wchar_t b)
{
    switch(a) {
        case L'¤¡':
            if(b == L'¤µ') {
                return L'¤£';
            }
            return L'\0';

        case L'¤¤':
            switch(b) {
                case L'¤¸': return L'¤¥';
                case L'¤¾': return L'¤¦';
                default: return L'\0';
            }

        case L'¤©':
            switch(b) {
                case L'¤¡': return L'¤ª';
                case L'¤±': return L'¤«';
                case L'¤²': return L'¤¬';
                case L'¤µ': return L'¤­';
                case L'¤¼': return L'¤®';
                case L'¤½': return L'¤¯';
                case L'¤¾': return L'¤°';
                default: return L'\0';
            }

        case L'¤²':
            if(b == L'¤µ') return L'¤´';
            return '\0';

        default: return '\0';
    }
}

// EX) ¤£ => ¤¡, Failed: Return param as it is
wchar_t KR::GetBeforeEnteredWord(IN wchar_t ch)
{
    switch(ch) {
            // Vowels
        case L'¤È': return L'¤Ç';
        case L'¤É': return L'¤Ç';
        case L'¤Ê': return L'¤Ç';
        case L'¤Í': return L'¤Ì';
        case L'¤Î': return L'¤Ì';
        case L'¤Ï': return L'¤Ì';
        case L'¤Ò':
            return L'¤Ñ';
            // consona
        case L'¤£': return L'¤¡';
        case L'¤¥': return L'¤¤';
        case L'¤¦': return L'¤¤';
        case L'¤ª': return L'¤©';
        case L'¤«': return L'¤©';
        case L'¤­': return L'¤©';
        case L'¤®': return L'¤©';
        case L'¤¯': return L'¤©';
        case L'¤°': return L'¤©';
        case L'¤´':
            return L'¤²';
            // default
        default: return ch;
    }
}

// EX) ¤£ => return ¤µ, Failed: Return null
wchar_t KR::GetAfterEnteredWord(IN wchar_t ch)
{
    switch(ch) {
            // Vowels
        case L'¤È': return L'¤¿';
        case L'¤É': return L'¤À';
        case L'¤Ê': return L'¤Ó';
        case L'¤Í': return L'¤Ã';
        case L'¤Î': return L'¤Ä';
        case L'¤Ï': return L'¤Ó';
        case L'¤Ò':
            return L'¤Ó';
            // consonants
        case L'¤£': return L'¤µ';
        case L'¤¥': return L'¤¸';
        case L'¤¦': return L'¤¾';
        case L'¤ª': return L'¤¡';
        case L'¤«': return L'¤±';
        case L'¤­': return L'¤µ';
        case L'¤®': return L'¤¼';
        case L'¤¯': return L'¤½';
        case L'¤°': return L'¤Ç';
        case L'¤´':
            return L'¤µ';
            // default
        default: return '\0';
    }
}

// Return: Before entered (¤Ò => ¤Ñ)
// Out(Optional): After entered (¤Ò => ¤Ó)
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

// EX) ¤¡,¤¿ ... => true, °¡, A ... => false
bool KR::IsDecomposed(IN wchar_t ch)
{
    if(ch < L'¤¡' || ch > L'¤Ó') {
        return false;
    }
    return true;
}

KR::EKinds KR::GetKinds(IN wchar_t ch)
{
    if(true == IsDecomposed(ch)) {
        if(ch <= L'¤¾') {
            return EKinds::CONSONANTS;
        }
        if(ch >= L'¤¿') {
            return EKinds::VOWELS;
        }
    }
    if(ch >= L'°¡' || ch <= L'ÆR') return EKinds::COMPLITION;
    return EKinds::NOT_KOR;
}

/*
    KR::Buffer
*/

// EX) [¤¿ + ¤¿]
wchar_t KR::Buffer::VowelsMixFailed(IN wchar_t input)
{
    wchar_t retval;
    switch(_cursor) {
            // EX) [¤¿ => ¤Ã]: keep ¤Ã, retrun ¤¿
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [¤¡ => ¤¿ => ¤Ã]: keep ¤Ã, return °¡
        case 1:
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1]); // EX) ¤¡ + ¤¿
            _buffer._array[0] = input;                                                // EX) ¤Ã
            _cursor           = 0;                                                    // Next Word
            return retval;

            // case 2: { ¤¡, ¤¿, ¤¿ }: Impossible
        default: return L'\0';
    }
}

// EX) [¤¡ => ¤¿]
wchar_t KR::Buffer::InputVowelAfterConsonant(IN wchar_t input)
{
    wchar_t lastInput = _buffer._array[_cursor];
    wchar_t retval    = L'\0';

    switch(_cursor) {
        case 0:
            // EX) [¤£ => ¤¿]: Keep »ç, return ¤¡
            if(EXCEPT == Table::GetInitialIndex(lastInput)) {
                wchar_t after, before = Decompose(_buffer._array[0], &after);

                retval            = before; // EX) ¤¡
                _buffer._array[0] = after;  // EX) ¤µ
                _buffer._array[1] = input;  // EX) ¤¿
                _cursor           = 1;      // Next word
                return retval;
            }

            // Else
            // EX) [¤¡ => ¤¿]: Keep °¡
            ++_cursor; // Add input word
            _buffer._array[_cursor] = input;
            return retval;

        case 2:
            // Ex) [¤¡ => ¤¿ => ¤¡ => ¤µ => ¤¿]: Keep »ç, return °¢
            if(EXCEPT == Table::GetInitialIndex(lastInput)) {
                wchar_t after, before = Decompose(lastInput, &after);

                retval            = Table::Combine(_buffer._array[0], _buffer._array[1], before); // EX) °¢
                _buffer._array[0] = after;                                                        // EX) ¤µ
                _buffer._array[1] = input;                                                        // EX) ¤¿
                _cursor           = 1;                                                            // Next word
                return retval;
            }

            // Else
            // EX) [¤¡ => ¤¿ => ¤¤ => ¤¿]: Keep ³ª, return °¡
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1]); // EX) °¡
            _buffer._array[0] = _buffer._array[2];                                    // EX) ¤¤
            _buffer._array[1] = input;                                                // EX) ¤¿
            _cursor           = 1;                                                    // Next Word
            return retval;

            // case 1: { ¤¿, ¤¡, ¤¿ + input }: Impossible
        default: return L'\0'; // Unused
    }
}

// EX) [¤¡ => ¤¤]
wchar_t KR::Buffer::ConsonantsMixFailed(IN wchar_t input)
{
    wchar_t retval;

    switch(_cursor) {
            // EX) [¤¡ => ¤¤]: Keep ¤¤, return ¤¡
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [¤¡ => ¤¿ => ¤¡ => ¤µ => ¤¤]: Keep ¤¤, return B
        case 2:
            retval            = Table::Combine(_buffer._array[0], _buffer._array[1], _buffer._array[2]);
            _buffer._array[0] = input; // Next
            _cursor           = 0;     // Next word
            return retval;

            // case 1: { ¤¡, ¤¤ + input }: Impossible
        default: return L'\0'; // Unused
    }
}

// EX) [¤¿ => ¤¡]
wchar_t KR::Buffer::InputConsonantsAfterVowels(IN wchar_t input)
{
    wchar_t retval;
    switch(_cursor) {
            // EX) [¤¿ => ¤¡]: Keep ¤¡, return ¤¿
        case 0:
            retval            = _buffer._array[0];
            _buffer._array[0] = input;
            return retval;

            // EX) [¤¡ =>  (¤Ç => ¤¿) => ¤¡]: Keep °û
        case 1:
            ++_cursor; // Add input word
            _buffer._array[_cursor] = input;
            return L'\0';

            // case 2: { ¤¿, ¤¡, ¤¿ + ¤¡ }: Impossible
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
            wchar_t mixed = CombineMiddle(lastInput, input); // EX) ¤Ñ +¤Ó = ¤Ò
            if(mixed == 0)                                   // Failed, EX) ¤¿ + ¤¿ = Null
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
        wchar_t mixed = CombineFinal(lastInput, input); // EX) ¤¡ + ¤µ = ¤£
        if(mixed == 0)                                  // Failed, EX)  ¤¡ + ¤¤ = Null
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

    // EX) ¤£ => ¤¡ OR ¤Ò => ¤Ñ
    wchar_t removed = GetBeforeEnteredWord(last);

    // Not decompose => Remove (EX) ¤¡ => ¤¡)
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