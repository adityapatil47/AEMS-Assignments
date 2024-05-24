// Sobel

#include <iostream>
#include <fstream>
#include <cmath>

#define WIDTH 256
#define HEIGHT 256

void applySobelFilter(unsigned char *input, unsigned char *output, int width, int height)
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};

    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int sumX = 0;
            int sumY = 0;

            for (int x = -1; x <= 1; x++)
            {
                for (int y = -1; y <= 1; y++)
                {
                    sumX += input[(i + x) * width + (j + y)] * Gx[x + 1][y + 1];
                    sumY += input[(i + x) * width + (j + y)] * Gy[x + 1][y + 1];
                }
            }

            int magnitude = (int)std::sqrt((sumX * sumX) + (sumY * sumY));
            if (magnitude > 255)
            {
                magnitude = 255;
            }
            else if (magnitude < 0)
            {
                magnitude = 0;
            }

            output[i * width + j] = (unsigned char)magnitude;
        }
    }
}

int main()
{
    std::ifstream inputFile("C:\\Users\\micro\\Desktop\\AEMS\\Images\\image1.raw", std::ios::binary);
    if (!inputFile.is_open())
    {
        std::cerr << "Cannot open input file" << std::endl;
        return 1;
    }

    std::ofstream outputFile("output_sobel.raw", std::ios::binary);
    if (!outputFile.is_open())
    {
        std::cerr << "Cannot open output file" << std::endl;
        inputFile.close();
        return 1;
    }

    unsigned char *inputImage = new unsigned char[WIDTH * HEIGHT];
    unsigned char *outputImage = new unsigned char[WIDTH * HEIGHT];

    inputFile.read(reinterpret_cast<char *>(inputImage), WIDTH * HEIGHT);
    inputFile.close();

    applySobelFilter(inputImage, outputImage, WIDTH, HEIGHT);

    outputFile.write(reinterpret_cast<const char *>(outputImage), WIDTH * HEIGHT);
    outputFile.close();

    delete[] inputImage;
    delete[] outputImage;

    std::cout << "Sobel filter applied and output saved to output_sobel.raw" << std::endl;

    return 0;
}
