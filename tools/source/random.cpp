#include "../include/random.h"

namespace sbl {

WordPtr RandomGenerator::nonCriticalSection = 0;

void RandomGenerator::Run(IN void* nCount)
{
    for(int i = 0; i < reinterpret_cast<WordPtr>(nCount); ++i) {
        ++nonCriticalSection;
    }
}

RandomGenerator::RandomGenerator()
{
    int loop = 32 >> 2;
    for(int i = 0; i < loop; ++i) {
        Block256 temp;
        UInt32* ptr = temp;

        ptr[i] = static_cast<UInt32>(PerfectRandom());
        rand.Initialize(temp);
    }
}

RandomGenerator::RandomGenerator(IN UInt64 seed)
{
    rand.Initialize(seed);
}

RandomGenerator::RandomGenerator(IN Block256 seed)
{
    rand.Initialize(seed);
}

void RandomGenerator::Initialize(IN UInt64 seed)
{
    rand.Initialize(seed);
}

void RandomGenerator::Initialize(IN Block256 seed)
{
    rand.Initialize(seed);
}

// return 0 ~ 1
float RandomGenerator::Next()
{
    readonly float CORRECT = 1.f / static_cast<UInt64>(-1);

    return rand.Generate() * CORRECT; // return value / max, Max: 1
}

float RandomGenerator::Next(IN float max)
{
    float val = Next();
    if(val == 1) {
        return 0;
    }
    return val * max;
}

float RandomGenerator::Next(IN float min, IN float max)
{
    float val = Next() + min;
    if(val == min + 1) {
        return 0;
    }
    return val * (max - min);
}

WordPtr RandomGenerator::PerfectRandom()
{
    nonCriticalSection = 0;
    _beginthread(Run, NULL, reinterpret_cast<void*>(0x7FFFF));
    Run(reinterpret_cast<void*>(0xFFFFF));
    return nonCriticalSection;
}

}; // namespace sbl
