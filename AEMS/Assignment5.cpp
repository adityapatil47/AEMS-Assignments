// Sum of Absolute Differance (SAD)

#include <iostream>
#include <fstream>
#include <cstdint>
#include <immintrin.h>
#include <intrin.h>

void findSadBlock(unsigned char *block1, unsigned char *block2, int width, int height, int *ptrSad)
{
    int blockSize = 64;
    int numBlocksX = width / blockSize;
    int numBlocksY = height / blockSize;

    for (int by = 0; by < numBlocksY; by++)
    {
        for (int bx = 0; bx < numBlocksX; bx++)
        {
            int sad = 0;
            for (int y = 0; y < blockSize; y++)
            {
                for (int x = 0; x < blockSize; x += 8)
                {
                    __m256i b1 = _mm256_loadu_si256((__m256i *)(block1 + (by * blockSize + y) * width + bx * blockSize + x));
                    __m256i b2 = _mm256_loadu_si256((__m256i *)(block2 + (by * blockSize + y) * width + bx * blockSize + x));
                    __m256i diff = _mm256_sad_epu8(b1, b2);
                    sad += _mm256_extract_epi16(diff, 0) + _mm256_extract_epi16(diff, 4);
                }
            }
            ptrSad[by * numBlocksX + bx] = sad;
        }
    }
}

int findSadFrame(int *ptrSad, int *blockX, int *blockY, int width, int height)
{
    int blockSize = 64;
    int numBlocksX = width / blockSize;
    int numBlocksY = height / blockSize;

    int minSad = ptrSad[0];
    *blockX = 0;
    *blockY = 0;

    for (int by = 0; by < numBlocksY; by++)
    {
        for (int bx = 0; bx < numBlocksX; bx++)
        {
            int sad = ptrSad[by * numBlocksX + bx];
            if (sad < minSad)
            {
                minSad = sad;
                *blockX = bx * blockSize;
                *blockY = by * blockSize;
            }
        }
    }

    return minSad;
}

int main()
{
    int width = 256;
    int height = 256;
    unsigned char *image1 = new unsigned char[width * height];
    unsigned char *image2 = new unsigned char[width * height];

    // Read image data into buffers
    std::ifstream file1("C:\\Users\\micro\\Desktop\\AEMS\\Images\\image1.raw", std::ios::binary);
    file1.read(reinterpret_cast<char *>(image1), width * height);
    file1.close();

    std::ifstream file2("C:\\Users\\micro\\Desktop\\AEMS\\Images\\image2.raw", std::ios::binary);
    file2.read(reinterpret_cast<char *>(image2), width * height);
    file2.close();

    int numBlocks = (width * height) / (64 * 64);
    int *sadArray = new int[numBlocks];

    uint64_t start = __rdtsc();
    findSadBlock(image1, image2, width, height, sadArray);
    int blockX, blockY;
    int minSad = findSadFrame(sadArray, &blockX, &blockY, width, height);
    uint64_t end = __rdtsc();

    std::cout << "Least SAD: " << minSad << " at block (" << blockX << ", " << blockY << ")" << std::endl;
    std::cout << "Cycles elapsed: " << (end - start) << std::endl;

    delete[] image1;
    delete[] image2;
    delete[] sadArray;

    return 0;
}
