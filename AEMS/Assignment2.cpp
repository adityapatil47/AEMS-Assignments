// Convolution DCT Quantization

#include <iostream>
#include <cmath>
#include <iomanip>

#define SIZE 8
#define QUALITY 2
#define M_PI 3.14159265358979323846
#define KERNEL_SIZE 3

int original_matrix[SIZE][SIZE] = {
    {140, 144, 147, 140, 140, 155, 179, 175},
    {144, 152, 140, 147, 140, 148, 167, 179},
    {152, 155, 136, 167, 163, 162, 152, 172},
    {168, 145, 156, 160, 152, 155, 136, 160},
    {162, 148, 156, 148, 140, 136, 147, 162},
    {147, 167, 140, 155, 155, 140, 136, 162},
    {136, 156, 123, 167, 162, 144, 140, 147},
    {148, 155, 136, 155, 152, 147, 147, 136}};

int kernel[KERNEL_SIZE][KERNEL_SIZE] = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 1, 1}};

int convolve(int row, int col)
{
    int sum = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int row_index = row + i;
            int col_index = col + j;
            if (row_index >= 0 && row_index < SIZE && col_index >= 0 && col_index < SIZE)
            {
                sum += original_matrix[row_index][col_index] * kernel[i + 1][j + 1];
            }
        }
    }
    return sum;
}

void shiftMatrix(int matrix[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            matrix[i][j] -= 128;
        }
    }
}

void performDCT(int matrix[SIZE][SIZE], int dctMatrix[SIZE][SIZE])
{
    for (int u = 0; u < SIZE; ++u)
    {
        for (int v = 0; v < SIZE; ++v)
        {
            double sum = 0.0;
            for (int x = 0; x < SIZE; ++x)
            {
                for (int y = 0; y < SIZE; ++y)
                {
                    sum += matrix[x][y] *
                           cos((2 * x + 1) * u * M_PI / (2 * SIZE)) *
                           cos((2 * y + 1) * v * M_PI / (2 * SIZE));
                }
            }
            double c_u = (u == 0) ? 0.707 : 1.0;
            double c_v = (v == 0) ? 0.707 : 1.0;
            dctMatrix[u][v] = 0.25 * c_u * c_v * sum;
        }
    }
}

void generateQuantizationMatrix(int qMatrix[SIZE][SIZE], int quality)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            qMatrix[i][j] = 1 + ((i + j + 1) * quality);
        }
    }
}

void quantizeMatrix(int dctMatrix[SIZE][SIZE], int qMatrix[SIZE][SIZE], int quantMatrix[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            quantMatrix[i][j] = dctMatrix[i][j] / qMatrix[i][j];
        }
    }
}

void zigzagScan(int matrix[SIZE][SIZE], int zigzagArray[SIZE * SIZE])
{
    int index = 0;
    for (int s = 0; s <= 2 * (SIZE - 1); ++s)
    {
        if (s % 2 == 0)
        {
            for (int i = 0; i <= s; ++i)
            {
                int j = s - i;
                if (i < SIZE && j < SIZE)
                {
                    zigzagArray[index++] = matrix[i][j];
                }
            }
        }
        else
        {
            for (int i = 0; i <= s; ++i)
            {
                int j = s - i;
                if (j < SIZE && i < SIZE)
                {
                    zigzagArray[index++] = matrix[j][i];
                }
            }
        }
    }
}
void printMatrix(int matrix[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            std::cout << std::setw(4) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printArray(int array[], int length)
{
    for (int i = 0; i < length; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    int shifted_matrix[SIZE][SIZE];
    int dct_matrix[SIZE][SIZE];
    int q_matrix[SIZE][SIZE];
    int quant_matrix[SIZE][SIZE];
    int zigzag_array[SIZE * SIZE];
    int convoluted_matrix[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            convoluted_matrix[i][j] = convolve(i, j);
        }
    }

    std::cout << "\nOriginal Matrix:" << std::endl;

    printMatrix(original_matrix);
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            shifted_matrix[i][j] = original_matrix[i][j] - 128;
        }
    }

    std::cout << "\nConvoluted Matrix:" << std::endl;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            std::cout << convoluted_matrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    std::cout << "\nShifted Matrix:" << std::endl;
    printMatrix(shifted_matrix);

    performDCT(shifted_matrix, dct_matrix);
    std::cout << "\nDCT Matrix:" << std::endl;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            std::cout << std::setw(4) << dct_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    generateQuantizationMatrix(q_matrix, QUALITY);
    std::cout << "\nQuantization Matrix:" << std::endl;
    printMatrix(q_matrix);

    quantizeMatrix(dct_matrix, q_matrix, quant_matrix);
    std::cout << "\nQuantized Matrix:" << std::endl;
    printMatrix(quant_matrix);

    zigzagScan(quant_matrix, zigzag_array);
    std::cout << "\nZigzag Scanned Array:" << std::endl;
    printArray(zigzag_array, SIZE * SIZE);
    return 0;
}
