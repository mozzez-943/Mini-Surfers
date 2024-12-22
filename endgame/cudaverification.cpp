#include <iostream>
#include <cuda_runtime.h>
#include <GL/gl.h>
#include <vector>

struct Obstacle {
    float x, y, size;
};

std::vector<Obstacle> obstacles;

// simply testing if cuda is working
int main() {
    int deviceCount;
    cudaGetDeviceCount(&deviceCount);
    if (deviceCount == 0) {
        std::cerr << "No CUDA devices found" << std::endl;
        return 1;
    }
    std::cout << "Found " << deviceCount << " CUDA devices" << std::endl;
    return 0;
}

// Render obstacles
void renderObstacles() {
    glColor3f(1.0f, 0.0f, 0.0f);
    for (const auto &obstacle : obstacles) {
        glBegin(GL_QUADS);
        glVertex2f(obstacle.x, obstacle.y);
        glVertex2f(obstacle.x + obstacle.size, obstacle.y);
        glVertex2f(obstacle.x + obstacle.size, obstacle.y + obstacle.size);
        glVertex2f(obstacle.x, obstacle.y + obstacle.size);
        glEnd();
    }
}