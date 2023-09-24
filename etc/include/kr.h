#pragma once

/*
    Last: 2023.4.25
    FILE ENCODING: ANSI (euc-KR)
    CODE ENCODING: UNICODE (UTF-8)
*/

#include "string"
#include "../../common/include/common.h"

class KR
{
public:
    enum EKinds
    {
        CONSONANTS,
        VOWELS,
        COMPLITION,
        NOT_KOR
    };

    struct Word
    {
        union
        {
            struct
            {
                wchar_t _initial;
                wchar_t _middle;
                wchar_t _final;
            };

            wchar_t _array[3];
        };
    };

    static class Table
    {
    public:
        // Consonants
        readonly wchar_t CHAR_LIST_INITIAL[] = { L'ぁ', L'あ', L'い', L'ぇ', L'え', L'ぉ', L'け', L'げ', L'こ', L'さ',
                                                 L'ざ', L'し', L'じ', L'す', L'ず', L'せ', L'ぜ', L'そ', L'ぞ' };

        // Vowels
        readonly wchar_t CHAR_LIST_MIDDLE[] = { L'た', L'だ', L'ち', L'ぢ', L'っ', L'つ', L'づ',
                                                L'て', L'で', L'と', L'ど', L'な', L'に', L'ぬ',
                                                L'ね', L'の', L'は', L'ば', L'ぱ', L'ひ', L'び' };

        // Consonants => Null: Empty, for counting
        readonly wchar_t CHAR_LIST_FINAL[] = {
            L'\0', L'ぁ', L'あ', L'ぃ', L'い', L'ぅ', L'う', L'ぇ', L'ぉ', L'お', L'か', L'が', L'き', L'ぎ',
            L'く', L'ぐ', L'け', L'げ', L'ご', L'さ', L'ざ', 'し',  L'じ', L'ず', L'せ', L'ぜ', L'そ', L'ぞ',
        };

        // value to index; a value of -1 is not used.
        readonly int INDEX_INITIAL[] = { 0,  1, -1, 2, -1, -1, 3,  4,  5,  -1, -1, -1, -1, -1, -1,
                                         -1, 6, 7,  8, -1, 9,  10, 11, 12, 13, 14, 15, 16, 17, 18 };

        readonly int INDEX_MIDDLE[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

        readonly int INDEX_FINAL[] = { 0,  1,  2,  3,  4,  5,  6,  7,  -1, 8,  9,  10, 11, 12, 13, 14,
                                       15, 16, 17, -1, 18, 19, 20, 21, 22, -1, 23, 24, 25, 26, 27 };

        readonly int INITIAL_CHANGE_UNIT = 588; // Middle * Final characters amount
        readonly int MIDDLE_CHANGE_UNIT  = 28;  // Final characters amount

        static Word    Decompose(IN wchar_t ch); // 唖 to ぁ / た / ぁ
        static wchar_t Combine(IN wchar_t initial, IN wchar_t middle,
                               IN wchar_t final = L'\0'); // USING CHAR => 唖 from ぁ / た / ぁ

        /*
            Char to Index methods
        */

        static int GetInitialIndex(IN wchar_t initial);
        static int GetMiddleIndex(IN wchar_t middle);
        static int GetFinalIndex(IN wchar_t final);

        /*
            Index to Char methods
        */

        static wchar_t GetInitialWord(IN int index);
        static wchar_t GetMiddleWord(IN int index);
        static wchar_t GetFinalWord(IN int index);
    };

    class Buffer
    {
    public:
        Buffer(): _buffer{ 0 }, _cursor{ 0 } {}

    private:
        /*
            Input cases
        */

        wchar_t VowelsMixFailed(IN wchar_t input);
        wchar_t InputVowelAfterConsonant(IN wchar_t input);
        wchar_t ConsonantsMixFailed(IN wchar_t input);
        wchar_t InputConsonantsAfterVowels(IN wchar_t input);

    private:
        /*
            No validation, use a TryInput method
        */

        wchar_t Enter(IN wchar_t input);

    public:
        int     TryInput(IN wchar_t kr);
        wchar_t EndInput();
        wchar_t GetWord();
        bool    Remove();
        void    Clear() { _buffer = { L'\0' }; };
        bool    IsEmpty() { return _cursor == 0 && _buffer._array[0] == L'\0'; }

    private:
        Word _buffer;
        int  _cursor; // To write == Last written (For word mixing)
    };

public:
    static wchar_t CombineMiddle(IN wchar_t a, IN wchar_t b);
    static wchar_t CombineFinal(IN wchar_t a, IN wchar_t b);

public:
    static wchar_t GetBeforeEnteredWord(IN wchar_t ch);
    static wchar_t GetAfterEnteredWord(IN wchar_t ch);

public:
    static EKinds  GetKinds(IN wchar_t ch);
    static bool    IsDecomposed(IN wchar_t ch);
    static wchar_t Decompose(IN wchar_t ch, OUT OPT wchar_t* after = nullptr);

public:
    void         Append(wchar_t input);
    void         Remove();
    void         Clear();
    std::wstring GetWritten();

private:
    Buffer       _stream;
    std::wstring _written;
};