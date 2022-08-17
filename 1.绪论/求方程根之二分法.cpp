// author : zhoukang
// date   : 2022-08-14 14:07:46

#include "求方程根之二分法.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

namespace NumericalAnalysis {
    namespace Chapter1{
        float solve_equation_with_bin_search(float a, float b, float eps, F f)
        {
            float ya = f(a);
            float yb = f(b);

            assert(ya * yb <= 0);

            while (fabs(b - a) >= eps) {
                float c = (b + a) / 2;
                float yc = f(c);
                if (yc * ya > 0) {
                    a = c;
                } else {
                    b = c;
                }
            }

            return (b + a) / 2;
        }
    }
}

void test_solve_equation_with_bin_search()
{
    using namespace NumericalAnalysis;
    auto f = [](float x) -> float {
        float y = x + log(x) - 2.2;
        return y;
    };


    float a = 1, b = 2;
    float eps = 0.000001;
    float res = Chapter1::solve_equation_with_bin_search(a, b, eps, f);

    printf("二分法求解方程, 求解区间[%.3f, %.3f]，求解精度:%.6f, 函数方程f(x) = x + log(x) - 2.2.\n"
           "求解结果:%.3f\n", a, b, eps, res);
}