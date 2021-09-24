// enthropy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "precompiled.h"
#include "EntropyBitCount.h"
#include "EntropyAccumulator.h"

// Size of the primary enthropy block in 64-bit chunks.
static const int PRIMARY_BLOCK_SIZE = 4096;
// Maximum number of attempts to obtain enthropy bits from the CPU.
static const int MAX_ATTEMPTS = 5;

static const double CHANGE_THRESHOLD = 0.005;
static const int FORCED_REFRESH_INTERVAL = 100;

static bool isRdseedSupported()
{
    bool rdseed = false;
    int abcd[4]; // { EAX, EBX, ECX, EDX }

    __cpuid(abcd, 0);
    if (abcd[0] >= 0x07)
    {
        // Standard CPUID function 7 is supported
        // Bit 18 of EBX is RDSEED
        __cpuid(abcd, 0x07);
        rdseed = 0 != ((abcd[1] >> 18) | 0x1);
    }

    return rdseed;
}

static void stepRdseed()
{
    unsigned __int64 enthropy = 0;

    if (0 == _rdseed64_step(&enthropy))
    {
        std::cout << "No enthropy.";
    }
    else
    {
        std::cout << "Enthropy: " << __popcnt64(enthropy) << std::endl;
    }
}

static EntropyBitCount populatePrimaryBlock()
{
    EntropyBitCount block;

    for (int i = 0; i < PRIMARY_BLOCK_SIZE; ++i)
    {
        unsigned __int64 enthropy = 0;
        int attempt = 0;

        for (; 0 == _rdseed64_step(&enthropy) && attempt < MAX_ATTEMPTS; ++attempt)
        {
            // Do nothing
        }

        if (attempt < MAX_ATTEMPTS)
        {
            const int ones = static_cast<int>(__popcnt64(enthropy));
            block.addBlock(ones, sizeof(enthropy) * 8 - ones);
        }
    }

    return block;
}

static double calculateRatio(const EntropyBitCount& bits)
{
    return static_cast<double>(bits.ones()) / static_cast<double>(bits.zeros());
}

int main()
{
    if (!isRdseedSupported())
    {
        // RDSEED is ot supported
    }
    else
    {
        EntropyAccumulator fastEnthropy(10);
        EntropyAccumulator slowEnthropy(100);

        std::cout << "Press any key to exit" << std::endl;

        int noChangeCount = 0;
        double fastRatio = 0, slowRatio = 0;

        while (0 == _kbhit())
        {
            const EntropyBitCount entropyBits = populatePrimaryBlock();

            fastEnthropy.add(entropyBits);
            slowEnthropy.add(entropyBits);

            bool report = false;
            const double newFastRatio = calculateRatio(fastEnthropy.bits());
            const double newSlowRatio = calculateRatio(slowEnthropy.bits());

            if (abs(newFastRatio - fastRatio) >= CHANGE_THRESHOLD || abs(newSlowRatio - slowRatio) >= CHANGE_THRESHOLD)
            {
                report = true;
            }

            if (!report && ++noChangeCount >= FORCED_REFRESH_INTERVAL)
            {
                noChangeCount = 0;
                report = true;
            }

            if (report)
            {
                slowRatio = newSlowRatio;
                fastRatio = newFastRatio;
                _cprintf("Fast: %6.4lf; Slow: %6.4lf\r", fastRatio, slowRatio);
            }
        }

        std::cout << std::endl << "Thank you." << std::endl;
    }

    return 0;
}
