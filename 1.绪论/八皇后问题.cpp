// author : zhoukang
// date   : 2022-08-14 15:13:02

#include "八皇后问题.h"
#include <stack>
#include <iostream>
#include "../BasicDefine.h"

namespace NumericalAnalysis {
    namespace Chapter1 {
        // 正向斜线:看成斜率k=1, 反向斜线:看成斜率k=-1
        // 创建的slash_map数组是有序的，从[0,0]到[n-1,n-1]以k = 1，遍历，然后从[0,n-1]到[n-1,0]以k = -1遍历
        std::vector<Slash> create_slash_map(int n)
        {
            std::vector<Slash> slash_map;

            // 从[0,0]到[0,n-1]以k=1遍历
            int start_row = 0;
            int start_col = 0;
            for (start_col = 0; start_col < n; ++start_col) {
                int row = start_row;
                int col = start_col;
                Slash slash;
                slash.second = SlashType::SLASH_TYPE_POS;
                while (row < n && col >= 0) {
                    slash.first.emplace_back(std::make_pair(row, col));
                    ++row;
                    --col;
                }
                slash_map.emplace_back(slash);
            }

            // 从[1,n-1]到[n-1,n-1]以k=1遍历
            start_col = n - 1;
            for (start_row = 1; start_row < n; ++start_row) {
                int row = start_row;
                int col = start_col;
                Slash slash;
                slash.second = SlashType::SLASH_TYPE_POS;
                while (row < n && col >= 0) {
                    slash.first.emplace_back(std::make_pair(row, col));
                    ++row;
                    --col;
                }
                slash_map.emplace_back(slash);
            }

            // 从[0,n-1]到[0,0]以k=-1遍历
            start_row = 0;
            for (start_col = n - 1; start_col >= 0; --start_col) {
                int row = start_row;
                int col = start_col;
                Slash slash;
                slash.second = SlashType::SLASH_TYPE_NEG;
                while (row < n && col < n) {
                    slash.first.emplace_back(std::make_pair(row, col));
                    ++row;
                    ++col;
                }
                slash_map.emplace_back(slash);
            }

            // 从[1,0]到[n-1,0]以k=-1遍历
            start_col = 0;
            for (start_row = 1; start_row < n; ++start_row) {
                int row = start_row;
                int col = start_col;
                Slash slash;
                slash.second = SlashType::SLASH_TYPE_NEG;
                while (row < n && col < n) {
                    slash.first.emplace_back(std::make_pair(row, col));
                    ++row;
                    ++col;
                }
                slash_map.emplace_back(slash);
            }

            return slash_map;
        }

        void print_slash(const Slash& slash, int n)
        {
            int* arr_dim2 = new int[n * n]();
            for (auto coord : slash.first) {
                int index = coord.first * n + coord.second;
                if (arr_dim2[index] == 1) {
                    printf("error，存在重复点\n");
                    exit(0);
                }
                arr_dim2[index] = 1;
            }

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int index = i * n + j;
                    if (arr_dim2[index] == 1) {
                        printf("✦ ");
                    } else {
                        printf("✧ ");
                    }
                }
                printf("\n");
            }

            delete[] arr_dim2;
            arr_dim2 = nullptr;
        }

        void print_answer(const Answer& answer)
        {
            int n = answer.size();
            if (n <= 0) {
                return;
            }
            int* arr_dim2 = new int[n * n]();
            for (auto coord : answer) {
                int index = coord.first * n + coord.second;
                if (arr_dim2[index] == 1) {
                    perror("error，存在重复点\n");
                    exit(0);
                }
                printf("%d ", coord.second);
                arr_dim2[index] = 1;
            }
            printf("\n");

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int index = i * n + j;
                    if (arr_dim2[index] == 1) {
                        printf("✦ ");
                    } else {
                        printf("✧ ");
                    }
                }
                printf("\n");
            }

            delete[] arr_dim2;
            arr_dim2 = nullptr;
        }

        void print_answers(const std::vector<Answer>& answers)
        {
            std::cout << "解的数量 : " << answers.size() << std::endl;
            for (auto answer : answers) {
                Chapter1::print_answer(answer);
                std::cout << std::endl;
            }
        }

        std::vector<int> get_slash_indexs(int row, int col, int n)
        {
            // indexs 与 slash_map配套使用
            std::vector<int> indexs;

            // 计算正向索引
            int pos_index = row + col;
            indexs.emplace_back(pos_index);

            // 反向索引从2n -1开始
            int off = 2 * n - 1;

            // 计算反向索引
            int reverse_col = n - col - 1;
            int neg_index = row + reverse_col + off;
            indexs.emplace_back(neg_index);

            return indexs;
        }

        int get_pos_index(int row, int col)
        {
            // 计算正向索引
            int pos_index = row + col;
            return pos_index;
        }

        int get_neg_index(int row, int col, int n)
        {
            /*
            // 反向索引从2n -1开始
            int off = 2 * n - 1;

            // 计算反向索引
            int reverse_col = n - col - 1;
            int neg_index = row + reverse_col + off;
             */
            int neg_index = row - col + 3 * n - 2; // 根据上面公式计算后的结果
            return neg_index;
        }

        bool belong_to_same_slash(const Coord& lcoord, const Coord& rcoord)
        {
            // 判断正向索引是否相同，相同，则属于同一条斜线slash
            if (lcoord.first + lcoord.second == rcoord.first + rcoord.second) {
                return true;
            }

            // 判断反向索引是否相同，相同，则属于同一条斜线
            // int n = 0;
            //int l_neg_index = lcoord.first - lcoord.second + 3 * n - 2;
            //int r_neg_index = rcoord.first - rcoord.second + 3 * n - 2;
            if (lcoord.first - lcoord.second == rcoord.first - rcoord.second) {
                return true;
            }

            return false;
        }

        bool belong_to_same_vhline(const Coord& lcoord, const Coord& rcoord)
        {
            if (lcoord.first == rcoord.first || lcoord.second == rcoord.second)
                return true;
            return false;
        }

        bool is_conflict(const Answer& answer, const Coord& coord)
        {
            for (int i = 0; i < answer.size(); ++i) {
                // 斜线冲突
                if (belong_to_same_slash(coord, answer[i])) {
                    return true;
                }

                // 行列冲突
                if (belong_to_same_vhline(coord, answer[i])) {
                    return true;
                }
            }
            return false;
        }

        std::vector<Answer> queen(int n)
        {
            std::vector<Answer> answers;
            Answer answer;

            int row = 0;
            int col = 0;

            while (true) {
                if (row == 0 && col < n) {
                    ASSERT(answer.size() == 0, "answer.size() != 0\n");
                    answer.push_back(std::make_pair(row, col));
                    ++row;
                    col = 0;
                    continue;
                }

                if (row <= 0 && col >= n) { // 包含了n为负数的情况
                    break;
                }

                if (col >= n) { // row >0 && col>=n
                    row = answer[answer.size() - 1].first;
                    col = answer[answer.size() - 1].second;
                    answer.pop_back();
                    ++col;
                    continue;
                }

                Coord coord = std::make_pair(row, col);
                if (is_conflict(answer, coord)) {
                    ++col;
                    continue;
                }

                answer.emplace_back(coord);
                ++row;
                col = 0;

                if (row == n) {
                    // 找到一条路径
                    answers.emplace_back(answer);

                    // 回到上一行，开始寻找其他路径
                    answer.pop_back();
                    --row;
                    col = coord.second + 1;
                }
            }

            return answers;
        }

        Coord rotate_coord(const Coord& coord, int n)
        {
            Coord new_coord;
            // 旋转一次后，
            new_coord.first = (n - 1) - coord.second;
            new_coord.second = coord.first;
            return new_coord;
        }

        // 计算左右对称的坐标
        Coord mirror_coord_hdir(const Coord& coord, int n)
        {
            Coord mirror_coord;
            mirror_coord.first = coord.first;
            mirror_coord.second = n - 1 - coord.second;
            return mirror_coord;
        }

        Coord mirror_coord_vdir(const Coord& coord, int n)
        {
            Coord mirror_coord;
            mirror_coord.second = coord.second;
            mirror_coord.first = n - 1 - coord.first;
            return mirror_coord;
        }

        std::vector<Answer> get_all_mirror_answers(const Answer& answer)
        {
            std::vector<Answer> mirror_answers;
            int n = answer.size();
            if (n <= 0) {
                return mirror_answers;
            }

            Answer mirror_answer_hdir, mirror_answer_vdir;
            mirror_answer_hdir.resize(n);
            mirror_answer_vdir.resize(n);

            for (int i = 0; i < answer.size(); ++i) {
                Coord coord = answer[i];
                Coord mirror_coord_h = mirror_coord_hdir(coord, n);
                mirror_answer_hdir[i] = mirror_coord_h;

                Coord mirror_coord_v = mirror_coord_vdir(coord, n);
                mirror_answer_vdir[mirror_coord_v.first] = mirror_coord_v;
            }
            mirror_answers.emplace_back(mirror_answer_hdir);
            mirror_answers.emplace_back(mirror_answer_vdir);

            return mirror_answers;
        }

        Answer rotate_answer(const Answer& answer)
        {
            Answer new_answer;

            int n = answer.size();
            if (n <= 0) {
                return new_answer;
            }

            new_answer.resize(n);
            for (int i = 0; i < n; ++i) {
                Coord new_coord = rotate_coord(answer[i],
                                               n); // 原来[0,2]的坐标，在answer的第0个位置，旋转后变为[5,0]，应该存放到new_answer的第5个位置
                new_answer[new_coord.first] = rotate_coord(answer[i], n);
            }

            return new_answer;
        }

        std::vector<Answer> get_all_rotate_answers(const Answer& answer)
        {
            std::vector<Answer> rotate_answers;
            rotate_answers.push_back(answer);

            Answer tmp = answer;
            for (int i = 0; i < 3; ++i) {
                tmp = rotate_answer(tmp);
                rotate_answers.push_back(tmp);
            }
            return rotate_answers;
        }

        bool same_answer(const Answer& src_answer, const Answer& dest_answer)
        {
            int n = src_answer.size();
            if (n != dest_answer.size())
                return false;

            for (int i = 0; i < n; ++i) {
                if (src_answer[i].first != dest_answer[i].first || src_answer[i].second != dest_answer[i].second) {
                    return false;
                }
            }

            return true;
        }

        bool in_answers(const Answer& answer, const std::vector<Answer>& answers)
        {
            for (int i = 0; i < answers.size(); ++i) {
                if (same_answer(answer, answers[i]))
                    return true;
            }
            return false;
        }

        void remove_rotate_duplicate(std::vector<Answer>& answers)
        {
            if (answers.size() <= 0) {
                return;
            }

            for (int i = 0; i < answers.size(); ++i) {
                Answer answer_i = answers[i];
                auto rotate_answers_i = get_all_rotate_answers(answer_i);
                for (int j = i + 1; j < answers.size(); ++j) {
                    Answer answer_j = answers[j];
                    if (in_answers(answer_j, rotate_answers_i)) {
                        //  删除第j个元素
                        answers.erase(answers.begin() + j);
                        --j;

                        printf("存在旋转重复元素\n");
                        // 打印重复元素
                        print_answer(answer_i);
                        print_answer(answer_j);
                    }
                }
            }
        }

        void remove_mirror_duplicate(std::vector<Answer>& answers)
        {
            if (answers.size() <= 0) {
                return;
            }

            for (int i = 0; i < answers.size(); ++i) {
                Answer answer_i = answers[i];
                auto mirror_answers_i = get_all_mirror_answers(answer_i);
                for (int j = i + 1; j < answers.size(); ++j) {
                    Answer answer_j = answers[j];
                    if (in_answers(answer_j, mirror_answers_i)) {
                        //  删除第j个元素
                        answers.erase(answers.begin() + j);
                        --j;

                        printf("存在对称重复元素\n");
                        // 打印重复元素
                        print_answer(answer_i);
                        print_answer(answer_j);
                    }
                }
            }
        }
    }
}


void test_eight_queen()
{
    using namespace NumericalAnalysis;
    std::cout << "输入皇后算法的阶数:";
    int n;
    std::cin >> n;
    std::cout << std::endl;

    std::cout << "输出经过点(1," << n - 1 << ")的斜线" << std::endl;
    auto slash_map = Chapter1::create_slash_map(n);
    auto indexs = Chapter1::get_slash_indexs(1, n - 1, n);
    for (auto index:indexs) {
        print_slash(slash_map[index], n);
        printf("\n");
    }

    std::cout << std::endl;
    std::cout << "输出皇后算法所有解:" << std::endl;
    auto answers = Chapter1::queen(n);
    Chapter1::print_answers(answers);

    std::cout << std::endl;
    std::cout << "查看旋转重复\n" << std::endl;
    Chapter1::remove_rotate_duplicate(answers);
    std::cout << "去重后输出皇后算法所有解:" << std::endl;
    Chapter1::print_answers(answers);

    std::cout << std::endl;
    std::cout << "查看对称重复\n" << std::endl;
    Chapter1::remove_mirror_duplicate(answers);
    std::cout << "去重后输出皇后算法所有解:" << std::endl;
    Chapter1::print_answers(answers);
}