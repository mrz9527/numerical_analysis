// author : zhoukang
// date   : 2022-08-14 14:07:46

#ifndef NUMERICAL_ANALYSIS_solve_equation_with_bin_search_H
#define NUMERICAL_ANALYSIS_solve_equation_with_bin_search_H

namespace NumericalAnalysis {
    namespace Chapter1 {
        typedef float (*F)(float x);
        /**
         * 根据二分法，求解方根在指定区间的根（前提条件是有根存在）
         * @param a
         * @param b
         * @param eps
         * @param f
         * @return
         */
        float solve_equation_with_bin_search(float a, float b, float eps, F f);
    }
}

void test_solve_equation_with_bin_search();

#endif //NUMERICAL_ANALYSIS_二分法_H