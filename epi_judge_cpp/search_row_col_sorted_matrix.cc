#include <vector>
#include "test_framework/generic_test.h"
using std::vector;
struct Position {
  int x, y;
  bool operator<=(const Position &rhs) const {
    return x <= rhs.x && y <= rhs.y;
  }
};

int LowerBoundHorizontal(const vector<int> &A, int k, int start, int end) {
  while (start <= end) {
    auto mid = start + (end - start) / 2;
    if (A[mid] < k)
      start = mid + 1;
    else if (A[mid] == k)
      return mid;
    else
      end = mid - 1;
  }
  return start;
}

int LowerBoundVertical(const vector<vector<int>> &A, int k, int x, int start, int end) {
  while (start <= end) {
    auto mid = start + (end - start) / 2;
    if (A[mid][x] < k)
      start = mid + 1;
    else if (A[mid][x] == k)
      return mid;
    else
      end = mid - 1;
  }
  return start;
}

Position LowerBound(const vector<vector<int>> &A, int k, Position start, Position end) {
  while (start <= end) {
    Position mid = {start.x + (end.x - start.x) / 2, start.y + (end.y - start.y) / 2};
    if (A[mid.y][mid.x] < k)
      start = {mid.x + 1, mid.y + 1};
    else if (A[mid.y][mid.x] == k)
      return mid;
    else
      end = {mid.x - 1, mid.y - 1};
  }
  return start;
}

bool MatrixSearchHelper(const vector<vector<int>> &A, int k, Position start, Position end) {
  if (A[start.y][start.x] > k || A[end.y][end.x] < k)
    return false;
  Position found = start;
  if (start.y == end.y)
    found.x = LowerBoundHorizontal(A[start.y], k, start.x, end.x);
  else if (start.x == end.x)
    found.y = LowerBoundVertical(A, k, start.x, start.y, end.y);
  else {
    int min = std::min(end.x - start.x, end.y - start.y);
    Position square = {start.x + min, start.y + min};
    if (A[square.y][square.x] < k) {
      start = square.x == end.x ? Position{start.x, square.y + 1} : Position{square.x + 1, start.y};
      return MatrixSearchHelper(A, k, start, end);
    }
    found = LowerBound(A, k, start, square);
  }
  if (A[found.y][found.x] == k)
    return true;
  return MatrixSearchHelper(A, k, {found.x, start.y}, {end.x, found.y - 1})
      || MatrixSearchHelper(A, k, {start.x, found.y}, {found.x - 1, end.y});
}

bool MatrixSearch(const vector<vector<int>> &A, int k) {
  return MatrixSearchHelper(A, k, {0, 0}, {static_cast<int>(A[0].size() - 1), static_cast<int>(A.size() - 1)});
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A", "x"};
  return GenericTestMain(args, "search_row_col_sorted_matrix.cc",
                         "search_row_col_sorted_matrix.tsv", &MatrixSearch,
                         DefaultComparator{}, param_names);
}
