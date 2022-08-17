// author : zhoukang
// date   : 2022-08-14 14:59:48

#include <iostream>

#include "八皇后问题.h"
#include "求方程根之二分法.h"


#include "vector"


typedef void(* TestFunc)();
using TestFuncInfo = std::pair<TestFunc, const char*>;
std::vector<TestFuncInfo> testFuncs;

inline void AddFunc(TestFunc func, const char* comment)
{
    testFuncs.push_back(std::make_pair(func, comment));
}

void construct_testFuncs()
{
    AddFunc(test_solve_equation_with_bin_search, "二分法求解方程");
    AddFunc(test_eight_queen, "八皇后问题");
}

void print_testFuncs()
{
    int len = testFuncs.size();
    printf("\n测试实例列表\n");
    for (int i = 0; i < len; ++i) {
        printf("%d. %s\n", i + 1, testFuncs[i].second);
    }
}

int main()
{
    construct_testFuncs();
    int len = testFuncs.size();

    while (1) {
        print_testFuncs();
        printf("选择测试实例，输入0退出:\n");
        int index;
        std::cin >> index;

        if (index <= 0 || index >len) {
            break;
        } else {
            testFuncs[index - 1].first();
        }
    }
    return 0;
}