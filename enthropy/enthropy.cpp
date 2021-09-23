// enthropy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <intrin.h>

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

int main()
{
    if (!isRdseedSupported())
    {
        // RDSEED is ot supported
    }
    else
    {
        for (int i = 0; i < 16; ++i)
        {
            stepRdseed();
        }
    }

    return 0;
}
