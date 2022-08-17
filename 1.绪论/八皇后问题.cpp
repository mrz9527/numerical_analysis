// author : zhoukang
// date   : 2022-08-14 15:13:02

#include "八皇后问题.h"
#include <stack>
#include <iostream>

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

        void print_answer(const Answer& answer, int n)
        {
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
            std::deque<Coord> que;

            int start_row = 0;
            int start_col = 0;
            for (start_col = 0; start_col < n; ++start_col) {
                que.push_back(std::make_pair(start_row, start_col));
            }

            while (!que.empty()) {
                Answer answer;

                // 给定一个起点
                Coord start_coord = que.front();
                que.pop_front();

                answer.push_back(start_coord);
                // 从第一行开始
                int row = 1;
                int col = 0;
                while (answer.size() > 0) {
                    if (col >= n) {
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

                        // 试图寻找其他路径
                        //    先删除当前answer的最后一个元素，它的索引为row - 1，然后寻找当前行的后一个元素
                        // answer.erase(answer.begin() + row - 1);
                        answer.pop_back();
                        --row;
                        col = coord.second + 1;
                    }
                }
            }
            return answers;
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
    std::cout << "输出皇后算法的所有解:" << std::endl;
    auto answers = Chapter1::queen(n);
    std::cout << "解的数量 : " << answers.size() << std::endl;
    for (auto answer : answers) {
        Chapter1::print_answer(answer, n);
        std::cout << std::endl;
    }
}