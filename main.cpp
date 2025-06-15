#include <iostream>
#include <omp.h>
#include "convolution.hpp"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <input.pgm> <kernel.txt> <output.pgm>\n";
        return 1;
    }
    int width, height, maxval, ksize;
    auto image  = load_pgm(argv[1], width, height, maxval);
    auto kernel = load_kernel(argv[2], ksize);

    // Serial timing
    double t1 = omp_get_wtime();
    auto result_serial = convolve_serial(image, kernel, width, height, ksize, maxval);
    double t2 = omp_get_wtime();
    std::cout << "[Serial] Convolution time: " << (t2 - t1) << " seconds\n";

    // Parallel timing
    double t3 = omp_get_wtime();
    auto result_parallel = convolve(image, kernel, width, height, ksize, maxval);
    double t4 = omp_get_wtime();
    std::cout << "[Parallel] Convolution time: " << (t4 - t3) << " seconds\n";

    // Save results
    save_pgm(argv[3], result_parallel, width, height, maxval);

    return 0;
}
