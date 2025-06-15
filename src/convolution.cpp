#include "convolution.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <omp.h>
#include <limits>

// Kernel loader
std::vector<std::vector<float>> load_kernel(const std::string& filename, int& size) {
    std::ifstream file(filename);
    file >> size;
    std::vector<std::vector<float>> kernel(size, std::vector<float>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            file >> kernel[i][j];
    return kernel;
}

// PGM loader
std::vector<std::vector<int>> load_pgm(const std::string& filename, int& width, int& height, int& maxval) {
    std::ifstream file(filename, std::ios::binary);
    std::string magic;
    file >> magic;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (file.peek() == '#') file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    file >> width >> height >> maxval;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::vector<int>> image(height, std::vector<int>(width));
    if (magic == "P2") {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j)
                file >> image[i][j];
    } else if (magic == "P5") {
        for (int i = 0; i < height; ++i)
            for (int j = 0; j < width; ++j) {
                unsigned char pix;
                file.read(reinterpret_cast<char*>(&pix), 1);
                image[i][j] = static_cast<int>(pix);
            }
    } else {
        throw std::runtime_error("Unsupported PGM format: " + magic);
    }
    return image;
}

// PGM Writer
void save_pgm(const std::string& filename, const std::vector<std::vector<int>>& image, int width, int height, int maxval) {
    std::ofstream file(filename);
    file << "P2\n" << width << " " << height << "\n" << maxval << "\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j)
            file << image[i][j] << " ";
        file << "\n";
    }
}

// Serial Convolve
std::vector<std::vector<int>> convolve_serial(
    const std::vector<std::vector<int>>& image,
    const std::vector<std::vector<float>>& kernel,
    int width, int height, int ksize, int maxval
) {
    int pad = ksize / 2;
    std::vector<std::vector<int>> output(height, std::vector<int>(width, 0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0.0f;
            for (int ki = 0; ki < ksize; ++ki) {
                for (int kj = 0; kj < ksize; ++kj) {
                    int ni = i + ki - pad, nj = j + kj - pad;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                        sum += image[ni][nj] * kernel[ki][kj];
                }
            }
            output[i][j] = std::min(maxval, std::max(0, static_cast<int>(sum)));
        }
    }
    return output;
}

// Parallel Convolve
std::vector<std::vector<int>> convolve(
    const std::vector<std::vector<int>>& image,
    const std::vector<std::vector<float>>& kernel,
    int width, int height, int ksize, int maxval
) {
    int pad = ksize / 2;
    std::vector<std::vector<int>> output(height, std::vector<int>(width, 0));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float sum = 0.0f;
            for (int ki = 0; ki < ksize; ++ki) {
                for (int kj = 0; kj < ksize; ++kj) {
                    int ni = i + ki - pad, nj = j + kj - pad;
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                        sum += image[ni][nj] * kernel[ki][kj];
                }
            }
            output[i][j] = std::min(maxval, std::max(0, static_cast<int>(sum)));
        }
    }
    return output;
}
