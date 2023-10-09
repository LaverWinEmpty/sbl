#ifndef SBL_RANDOM_GENERATOR_HEADER__
#define SBL_RANDOM_GENERATOR_HEADER__

#include "thread"
#include "../../utilities/include/xoshiro256.h"

namespace sbl {

class RandomGenerator
{
public:
    RandomGenerator();
    RandomGenerator(IN uint64_t seed);
    RandomGenerator(IN Block256 seed);

public:
    void Initialize(IN uint64_t seed);
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
    static ssize_t PerfectRandom();

private:
    Xoshiro256 rand;

private:
    static void    Run(IN void* nCount);
    static ssize_t nonCriticalSection;
};

} // namespace sbl
#endif