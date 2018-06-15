#include "Crypto\ProofOfWork.h"

#include <cryptopp\osrng.h>
#include <blake2\blake2.h>
#include <Platform\WinUtils.h>

#include <cassert>
#include <omp.h>

using ByteArray32 = std::array<uint8_t, 32>;

class xorshift1024star
{
public:
    xorshift1024star() :
        p(0)
    {
    }
    std::array<uint64_t, 16> s;
    unsigned p;
    uint64_t next()
    {
        auto p_l(p);
        auto pn((p_l + 1) & 15);
        p = pn;
        uint64_t s0 = s[p_l];
        uint64_t s1 = s[pn];
        s1 ^= s1 << 31; // a
        s1 ^= s1 >> 11; // b
        s0 ^= s0 >> 30; // c
        return (s[pn] = s0 ^ s1) * 1181783497276652981LL;
    }
};

static uint64_t sizeOfWorkDone(const rail::ByteArray32 & hashedBlock, uint64_t workDone)
{
    uint64_t result;

    blake2b_state hash;
    blake2b_init(&hash, sizeof(result));
    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&workDone), sizeof(workDone));
    blake2b_update(&hash, hashedBlock.data(), hashedBlock.size());
    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&result), sizeof(result));

    return result;
}

/*static bool isWorkValid(const rail::ByteArray32 & hashedBlock, uint64_t workDone)
{
    return sizeOfWorkDone(hashedBlock, workDone) > PUBLISH_THRESHOLD;
}*/

uint64_t rail::Pow::generateWork(const rail::ByteArray32& inputHash)
{
    if (inputHash.size() == 0) return 0;

    uint64_t finishedWork{ 0 };
    bool finished{ false };


#pragma omp parallel shared (finishedWork, finished, inputHash) 
    {
        //WinUtils::ThreadUtils::setThreadBackgroundAffinity();

        xorshift1024star rng;
        CryptoPP::AutoSeededRandomPool randPool;
        randPool.GenerateBlock(reinterpret_cast<uint8_t *> (rng.s.data()), rng.s.size() * sizeof(decltype (rng.s)::value_type));

        uint64_t work{ 0 };
        uint64_t output{ 0 };
        blake2b_state hash;

        while (!finished && output < PUBLISH_THRESHOLD)
        {
            unsigned iteration(32);
            while (!finished && iteration && output < PUBLISH_THRESHOLD)
            {
                work = rng.next();
                blake2b_init(&hash, sizeof(output));
                blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                blake2b_update(&hash, inputHash.data(), inputHash.size());
                blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                if (output < PUBLISH_THRESHOLD)
                {
                    work = rng.next();
                    blake2b_init(&hash, sizeof(output));
                    blake2b_update(&hash, reinterpret_cast<uint8_t *> (&work), sizeof(work));
                    blake2b_update(&hash, inputHash.data(), inputHash.size());
                    blake2b_final(&hash, reinterpret_cast<uint8_t *> (&output), sizeof(output));
                }

                iteration -= 8;
            }

            if (output >= PUBLISH_THRESHOLD)
            {
                #pragma omp critical
                {
                    finishedWork = work;

                    if (sizeOfWorkDone(inputHash, work) == output)
                    {
                        finished = true;
                    }
                    else
                    {
                        assert(false);
                    }
                }
            }
        }
    }

    return finishedWork;
}