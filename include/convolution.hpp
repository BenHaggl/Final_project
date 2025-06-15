#pragma once
#include <vector>
#include <string>

// Kernel loader
std::vector<std::vector<float>> load_kernel(const std::string& filename, int& size);

// PGM loader
std::vector<std::vector<int>> load_pgm(const std::string& filename, int& width, int& height, int& maxval);

// PGM writer
void save_pgm(const std::string& filename, const std::vector<std::vector<int>>& image, int width, int height, int maxval);

// Serial convolution
std::vector<std::vector<int>> convolve_serial(
    const std::vector<std::vector<int>>& image,
    const std::vector<std::vector<float>>& kernel,
    int width, int height, int ksize, int maxval
);

// Parallel convolution
std::vector<std::vector<int>> convolve(
    const std::vector<std::vector<int>>& image,
    const std::vector<std::vector<float>>& kernel,
    int width, int height, int ksize, int maxval
);
