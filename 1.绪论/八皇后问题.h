// author : zhoukang
// date   : 2022-08-14 15:13:02

#ifndef NUMERICAL_ANALYSIS_queen_H
#define NUMERICAL_ANALYSIS_queen_H


#include "vector"

namespace NumericalAnalysis{
    namespace Chapter1{
        using Coord = std::pair<int, int>;
        using Answer = std::vector<Coord>;
        using SlashPath = Answer;
        enum class SlashType {
            SLASH_TYPE_POS = 0, // 正向斜线，比如[0,n-1] [1,n-2]...[n-1,0]
            SLASH_TYPE_NEG = 1  // 反向斜线，比如[0,0] [1,1] ...[n-1,n-1]
        };
        using Slash = std::pair<SlashPath, SlashType>;

        // 导出函数
        std::vector<Answer> queen(int n);
        void remove_rotate_duplicate(std::vector<Answer>& answers);
        void remove_mirror_duplicate(std::vector<Answer>& answers);

        std::vector<Slash> create_slash_map(int n);
        std::vector<int> get_slash_indexs(int row, int col, int n);

        void print_slash(const Slash& slash, int n);
        void print_answer(const Answer& answer);
    }
}

void test_eight_queen();

#endif //NUMERICAL_ANALYSIS_八皇后问题_H