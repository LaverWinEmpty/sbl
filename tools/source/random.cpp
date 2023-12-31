#include "../include/random.h"

namespace sbl {

ssize_t RandomGenerator::nonCriticalSection = 0;

void RandomGenerator::Run(IN void* nCount)
{
    for(int i = 0; i < reinterpret_cast<ssize_t>(nCount); ++i) {
        ++nonCriticalSection;
    }
}

RandomGenerator::RandomGenerator()
{
    int loop = 32 >> 2;
    for(int i = 0; i < loop; ++i) {
        Block256  temp;
        uint32_t* ptr = temp;

        ptr[i] = static_cast<uint32_t>(PerfectRandom());
        rand.Initialize(temp);
    }
}

RandomGenerator::RandomGenerator(IN uint64_t seed)
{
    rand.Initialize(seed);
}

RandomGenerator::RandomGenerator(IN Block256 seed)
{
    rand.Initialize(seed);
}

void RandomGenerator::Initialize(IN uint64_t seed)
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
    readonly float CORRECT = 1.f / static_cast<uint64_t>(-1);

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

ssize_t RandomGenerator::PerfectRandom()
{
    nonCriticalSection = 0;
    _beginthread(Run, NULL, reinterpret_cast<void*>(0x7FFFF));
    Run(reinterpret_cast<void*>(0xFFFFF));
    return nonCriticalSection;
}

}; // namespace sbl
