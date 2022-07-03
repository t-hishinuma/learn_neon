#include <iostream>
#include <vector>
#include <chrono>
#include <cassert>
#include <arm_neon.h>
#define ITER 300
#define ORDER 2*size/1000000000
#define SIMD_LEN 2

size_t check_ans(std::vector<double> &ret, std::vector<double> &ans){
    if(ret.size() != ans.size()){
        return 1;
    }

    for(size_t i=0; i<ans.size(); i++){
        if(ret[i] != ans[i]){
            std::cout 
                << i << ": "
                << "ret: " << ret[i] << ", "
                << "ans: " << ans[i] << std::endl;
            return 1;
        }
    }

    return 0;
}

void axpy_normal(double alpha, const std::vector<double> &x, std::vector<double> &y){
    for(size_t i = 0; i < x.size(); i++){
        y[i] += alpha * x[i];
    }
}

void axpy_neon(double alpha, const std::vector<double> &x, std::vector<double> &y){
    auto av = vld1q_dup_f64(&alpha);
    for(size_t i = 0; i < x.size() - (SIMD_LEN-1); i+=SIMD_LEN){
        auto xv = vld1q_f64(&x.data()[i]);
        auto yv = vld1q_f64(&y.data()[i]);

        auto tv = vmulq_f64(av, xv); //ax
        yv = vaddq_f64(yv, tv); // y+=ax
        vst1q_f64(&y.data()[i], yv);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "error, $1 = size " << std::endl;
        return 1;
    }

    size_t size = atoi(argv[1]);

    std::vector<double> x(size, 1.0);
    std::vector<double> y(size, 0.0);
    std::vector<double> ansy(size, 0.0);
    double alpha = 123.0;

    //check ans
    axpy_normal(alpha, x, ansy); 
    axpy_neon(alpha, x, y); 
    assert(check_ans(y, ansy)==0);

    {
        auto start = std::chrono::system_clock::now();
        for(size_t iter = 0; iter < ITER; iter++){
            axpy_normal(alpha, x, ansy); 
        }
        auto end = std::chrono::system_clock::now();
        double sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count() /
            1.0e+9;

        std::cout << "normal\t"<< sec/ITER << ", " << 2*size/(sec/ITER)/1000000000 << std::endl;
    }

    {
        auto start = std::chrono::system_clock::now();
        for(size_t iter = 0; iter < ITER; iter++){
            axpy_neon(alpha, x, ansy); 
        }
        auto end = std::chrono::system_clock::now();
        double sec = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count() /
            1.0e+9;

        std::cout << "neon\t"<< sec/ITER << ", " << 2*size/(sec/ITER)/1000000000 << std::endl;
    }
    return 0;
}
