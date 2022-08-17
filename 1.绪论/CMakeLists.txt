cmake_minimum_required(VERSION 3.5)
project(numerical_analysis)

set(CMAKE_CXX_STANDARD 11)

####################算法##########################
# 二分法求解方程
add_library(solve_equation_with_bin_search SHARED 求方程根之二分法.cpp)

# 八皇后问题
add_library(eight_queen SHARED 八皇后问题.cpp)


####################算法应用##########################
#二分法求解方程实例
add_executable(chapter1 chapter1.cpp)
target_link_libraries(chapter1
        solve_equation_with_bin_search
        eight_queen)