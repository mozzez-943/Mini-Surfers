#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

// Window dimensions
const int windowWidth = 800;
const int windowHeight = 600;

// Game constants
const int numObstacles = 10;
const float playerWidth = 50.0f;
const float playerHeight = 50.0f;

struct Obstacle {
    float x, y, size;
};

// Player position and obstacles
float playerX = windowWidth / 2 - playerWidth / 2;
float playerY = 50.0f;
std::vector<Obstacle> obstacles;

// CUDA device function to move obstacles
__global__ void moveObstacles(Obstacle *obstacles, int numObstacles, float speed) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < numObstacles) {
        obstacles[idx].y -= speed;
        if (obstacles[idx].y < 0) {
            obstacles[idx].y = 600;
            obstacles[idx].x = rand() % 750;
        }
    }
}

// Initialize obstacles
void initObstacles() {
    for (int i = 0; i < numObstacles; i++) {
        obstacles.push_back({static_cast<float>(rand() % (windowWidth - 50)),
                             static_cast<float>(rand() % (windowHeight / 2) + windowHeight / 2),
                             static_cast<float>(rand() % 20 + 30)});
    }
}

// Render the player
void renderPlayer() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(playerX, playerY);
    glVertex2f(playerX + playerWidth, playerY);
    glVertex2f(playerX + playerWidth, playerY + playerHeight);
    glVertex2f(playerX, playerY + playerHeight);
    glEnd();
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

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderPlayer();
    renderObstacles();
    glutSwapBuffers();
}

// Timer callback
void timer(int value) {
    // Allocate memory on the GPU for obstacles
    Obstacle *d_obstacles;
    size_t size = numObstacles * sizeof(Obstacle);
    cudaMalloc(&d_obstacles, size);
    cudaMemcpy(d_obstacles, obstacles.data(), size, cudaMemcpyHostToDevice);

    // Launch CUDA kernel to move obstacles
    int threadsPerBlock = 256;
    int numBlocks = (numObstacles + threadsPerBlock - 1) / threadsPerBlock;
    moveObstacles<<<numBlocks, threadsPerBlock>>>(d_obstacles, numObstacles, 5.0f);

    // Copy updated obstacle positions back to the host
    cudaMemcpy(obstacles.data(), d_obstacles, size, cudaMemcpyDeviceToHost);
    cudaFree(d_obstacles);

    // Redraw
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            playerX -= 10.0f;
            break;
        case 'd':
            playerX += 10.0f;
            break;
        case 27: // Escape key
            exit(0);
    }
}

// Main function
int main(int argc, char **argv) {
    srand(static_cast<unsigned>(time(0)));
    initObstacles();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("CUDA Subway Surfers");
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
