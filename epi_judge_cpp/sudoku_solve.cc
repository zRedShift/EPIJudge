#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::begin;
using std::end;
using std::vector;

const int kN = 9, kSqrtN = 3;
using Seen = std::array<std::bitset<kN>, kN>;

bool SolvePartialSudoku(vector<vector<int>> &sudoku, int x, int y, Seen &horizontal, Seen &vertical, Seen &subarray) {
  if (x == kN) {
    x = 0;
    if (++y == kN)
      return true;
  }
  if (sudoku[y][x])
    return SolvePartialSudoku(sudoku, x + 1, y, horizontal, vertical, subarray);
  const int subarray_index = y / kSqrtN * kSqrtN + x / kSqrtN;
  const std::bitset<kN> seen = horizontal[y] | vertical[x] | subarray[subarray_index];
  for (int i = 0; i < kN; ++i)
    if (!seen[i]) {
      horizontal[y].set(i);
      vertical[x].set(i);
      subarray[subarray_index].set(i);
      if (SolvePartialSudoku(sudoku, x + 1, y, horizontal, vertical, subarray)) {
        sudoku[y][x] = i + 1;
        return true;
      }
      horizontal[y].reset(i);
      vertical[x].reset(i);
      subarray[subarray_index].reset(i);
    }
  return false;
}

bool SolveSudoku(vector<vector<int>> *partial_assignment) {
  auto &sudoku = *partial_assignment;
  Seen horizontal, vertical, subarray;
  for (int y = 0; y < kN; ++y)
    for (int x = 0; x < kN; ++x)
      if (int val = sudoku[y][x]; val--) {
        int subarray_index = y / kSqrtN * kSqrtN + x / kSqrtN;
        if (horizontal[y][val] || vertical[x][val] || subarray[subarray_index][val])
          return false;
        horizontal[y].set(val);
        vertical[x].set(val);
        subarray[y / kSqrtN * kSqrtN + x / kSqrtN].set(val);
      }
  return SolvePartialSudoku(sudoku, 0, 0, horizontal, vertical, subarray);
}

vector<int> GatherColumn(const vector<vector<int>> &data, size_t i) {
  vector<int> result;
  for (auto &row : data) {
    result.push_back(row[i]);
  }
  return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>> &data,
                              size_t block_size, size_t n) {
  vector<int> result;
  size_t block_x = n % block_size;
  size_t block_y = n / block_size;
  for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
    for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
      result.push_back(data[i][j]);
    }
  }

  return result;
}

void AssertUniqueSeq(const vector<int> &seq) {
  vector<bool> seen(seq.size(), false);
  for (auto &x : seq) {
    if (x == 0) {
      throw TestFailure("Cell left uninitialized");
    }
    if (x < 0 || x > seq.size()) {
      throw TestFailure("Cell value out of range");
    }
    if (seen[x - 1]) {
      throw TestFailure("Duplicate value in section");
    }
    seen[x - 1] = true;
  }
}

void SolveSudokuWrapper(TimedExecutor &executor,
                        const vector<vector<int>> &partial_assignment) {
  vector<vector<int>> solved = partial_assignment;

  executor.Run([&] { SolveSudoku(&solved); });

  if (!std::equal(begin(partial_assignment), end(partial_assignment),
                  begin(solved), end(solved), [](auto br, auto cr) {
        return std::equal(begin(br), end(br), begin(cr), end(cr),
                          [](int bcell, int ccell) {
                            return bcell == 0 || bcell == ccell;
                          });
      }))
    throw TestFailure("Initial cell assignment has been changed");

  auto block_size = static_cast<size_t>(sqrt(solved.size()));

  for (size_t i = 0; i < solved.size(); i++) {
    AssertUniqueSeq(solved[i]);
    AssertUniqueSeq(GatherColumn(solved, i));
    AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "partial_assignment"};
  return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                         &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
