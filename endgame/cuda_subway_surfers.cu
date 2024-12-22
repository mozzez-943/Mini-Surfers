#include <iostream>

// CUDA kernel example
__global__ void helloFromGPU() {
    printf("Hello from the GPU!\n");
}

int main() {
    std::cout << "Hello from the CPU!" << std::endl;

    // Launch CUDA kernel
    helloFromGPU<<<1, 1>>>();
    cudaDeviceSynchronize();

    return 0;
}
