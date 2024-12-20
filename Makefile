# Makefile for CUDA Subway Surfers Game

# Compiler
NVCC = nvcc
GPP = g++

# Flags
CXXFLAGS = -I/usr/include -I/usr/local/cuda/include
LDFLAGS = -L/usr/lib -L/usr/local/cuda/lib64 -lGL -lGLU -lglut -lcuda -lcudart

# Source files
SRCS = cuda_subway_surfers.cu

# Output executable
TARGET = cuda_subway_surfers

# Default rule
all: verify_cuda $(TARGET)

# Build target
$(TARGET): $(SRCS)
	$(NVCC) $(SRCS) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# CUDA verification
verify_cuda:
	echo "Verifying CUDA environment..."
	@command -v nvcc >/dev/null 2>&1 || { echo "Error: nvcc is not installed or not in PATH."; exit 1; }
	@nvcc --version
	@echo "CUDA environment verified."

# Clean rule
clean:
	rm -f $(TARGET) *.o
