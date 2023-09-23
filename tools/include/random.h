#ifndef __SEMIBASE_RANDOM_TOOL_HEADER__
#define __SEMIBASE_RANDOM_TOOL_HEADER__

#include "thread"
#include "../../utilities/include/xoshiro256.h"

namespace sbl {

class RandomGenerator
{
public:
    RandomGenerator();
    RandomGenerator(IN UInt64 seed);
    RandomGenerator(IN Block256 seed);

public:
    void Initialize(IN UInt64 seed);
    void Initialize(IN Block256 seed);

public:
    float Next();
    float Next(IN float max);
    float Next(IN float min, IN float max);

public:
    // ### Get random value through race condition ###
    //
    // There is an error in a single-thread.
    // Safe even if called multiple times
    // - The child thread end before the procedure ends.
    //   So the variable are fixed after a function call.
    //   But may vary depending on the environment.
    //
    // Recommend: use for seed
    static WordPtr PerfectRandom();

private:
    Xoshiro256 rand;

private:
    static void    Run(IN void* nCount);
    static WordPtr nonCriticalSection;
};

} // namespace sbl
#endif