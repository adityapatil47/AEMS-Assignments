#include <iostream>
#include <fstream>
#include <cmath>
#include <c6x.h>

#define WIDTH 256
#define HEIGHT 256

void applySobelFilterC66x(unsigned char *input, unsigned char *output, int width, int height)
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
		for (int j = 1; j < width - 1; j += 8)
		{
			__x128_t sumX1 = _dspzero();
			__x128_t sumY1 = _dspzero();

			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					__x128_t pixels = _mem8_const(&input[(i + x) * width + (j + y)]);
					__x128_t gx_val = _pack2(Gx[x + 1][y + 1], Gx[x + 1][y + 1]);
					__x128_t gy_val = _pack2(Gy[x + 1][y + 1], Gy[x + 1][y + 1]);

					sumX1 = _dadd2(sumX1, _dmpy2(pixels, gx_val));
					sumY1 = _dadd2(sumY1, _dmpy2(pixels, gy_val));
				}
			}

			__x128_t sumX1_sq = _dmpy2(sumX1, sumX1);
			__x128_t sumY1_sq = _dmpy2(sumY1, sumY1);
			__x128_t magnitude_sq = _dadd2(sumX1_sq, sumY1_sq);
			__x128_t magnitude = _dspulimit(_dspsqrt(magnitude_sq), 255);

			_mem8(&output[i * width + j]) = magnitude;
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

	unsigned char *inputImage = (unsigned char *)_aligned_malloc(WIDTH * HEIGHT * sizeof(unsigned char), 64);
	unsigned char *outputImage = (unsigned char *)_aligned_malloc(WIDTH * HEIGHT * sizeof(unsigned char), 64);

	if (inputImage == nullptr || outputImage == nullptr)
	{
		std::cerr << "Memory allocation failed" << std::endl;
		inputFile.close();
		if (inputImage)
			_aligned_free(inputImage);
		if (outputImage)
			_aligned_free(outputImage);
		return 1;
	}

	inputFile.read(reinterpret_cast<char *>(inputImage), WIDTH * HEIGHT);
	inputFile.close();

	applySobelFilterC66x(inputImage, outputImage, WIDTH, HEIGHT);

	outputFile.write(reinterpret_cast<const char *>(outputImage), WIDTH * HEIGHT);
	outputFile.close();

	_aligned_free(inputImage);
	_aligned_free(outputImage);

	std::cout << "Sobel filter applied and output saved to output_sobel.raw" << std::endl;

	return 0;
}
