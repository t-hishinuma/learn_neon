#include <iostream>
#include <vector>
#include <chrono>
#define ITER 100
#define ORDER 2*size/1000000000

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "error, $1 = size " << std::endl;
        return 1;
    }

    size_t size = atoi(argv[1]);

    std::vector<double> x(size, 1.0);
    std::vector<double> y(size, 0.0);
    double alpha = 123.0;


  auto start = std::chrono::system_clock::now();
  for(size_t iter = 0; iter < ITER; iter++){
      for(size_t i = 0; i < x.size(); i++){
          y[i] += alpha * x[i];
      }
  }
  auto end = std::chrono::system_clock::now();
  double sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
                   .count() /
               1.0e+9;

  std::cout << sec/ITER << std::endl;
  std::cout << 2*size/(sec/ITER)/1000000000 << std::endl;

  return 0;
}
