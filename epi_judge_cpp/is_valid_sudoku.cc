#include "test_framework/generic_test.h"
// Check if a partially filled matrix has any conflicts.
bool IsValidSudoku(const std::vector<std::vector<int>> &partial_assignment) {
  const auto kN = 9;
  const auto kSqrtN = 3;
  int val;
  for (auto i = 0; i < kN; ++i) {
    std::array<char, 3 * kN> seen{0};
    for (auto j = 0; j < kN; ++j) {
      val = partial_assignment[i][j];
      if (val && seen[val - 1]++)
        return false;
      val = partial_assignment[j][i];
      if (val && seen[val - 1 + kN]++)
        return false;
      val = partial_assignment[i / kSqrtN * kSqrtN + j / kSqrtN][i % kSqrtN * kSqrtN + j % kSqrtN];
      if (val && seen[val - 1 + 2 * kN]++)
        return false;
    }
  }
  return true;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"partial_assignment"};
  return GenericTestMain(args, "is_valid_sudoku.cc", "is_valid_sudoku.tsv",
                         &IsValidSudoku, DefaultComparator{}, param_names);
}
