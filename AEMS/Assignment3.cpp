#include <iostream>
#include <fstream>
#include <cstdlib>

void convertToGrayscale(unsigned char *input, unsigned char *output, int width, int height, int channels)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = (y * width + x) * channels;
            unsigned char r = input[i];
            unsigned char g = input[i + 1];
            unsigned char b = input[i + 2];
            unsigned char gray = (unsigned char)(0.3f * r + 0.59f * g + 0.11f * b);
            output[y * width + x] = gray;
        }
    }
}

int main()
{
    const char *inputFilename = "C:\\Users\\micro\\Desktop\\AEMS\\Images\\airplane.raw";
    const char *outputFilename = "C:\\Users\\micro\\Desktop\\AEMS\\Images\\grayscale_airplane.raw";
    const int width = 512;
    const int height = 512;
    const int channels = 3;

    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "Error opening input file\n";
        return EXIT_FAILURE;
    }

    unsigned char *image = new unsigned char[width * height * channels];
    if (!image)
    {
        std::cerr << "Error allocating memory for image\n";
        inputFile.close();
        return EXIT_FAILURE;
    }

    inputFile.read(reinterpret_cast<char *>(image), width * height * channels);
    inputFile.close();

    unsigned char *grayscaleImage = new unsigned char[width * height];
    if (!grayscaleImage)
    {
        std::cerr << "Error allocating memory for grayscale image\n";
        delete[] image;
        return EXIT_FAILURE;
    }

    convertToGrayscale(image, grayscaleImage, width, height, channels);

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error opening output file\n";
        delete[] image;
        delete[] grayscaleImage;
        return EXIT_FAILURE;
    }

    outputFile.write(reinterpret_cast<char *>(grayscaleImage), width * height);
    outputFile.close();

    std::cout << "Image processed and saved to " << outputFilename << std::endl;

    delete[] image;
    delete[] grayscaleImage;

    return EXIT_SUCCESS;
}
