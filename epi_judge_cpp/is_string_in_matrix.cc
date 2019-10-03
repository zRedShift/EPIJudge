#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

namespace std {
template<>
struct hash<tuple<int, int, int>> {
  size_t operator()(const tuple<int, int, int> &x) const {
    return ((size_t(get<0>(x)) << 24u) ^ get<1>(x) << 16u) ^ get<2>(x);
  }
};
}

bool MatchPattern(const vector<vector<int>> &grid,
                  const vector<int> &pattern,
                  const int x,
                  const int y,
                  int i,
                  std::unordered_set<std::tuple<int, int, int>> &matches) {
  if (i == pattern.size())
    return true;
  if (y < 0 || y == grid.size() || x < 0 || x == grid.front().size() || grid[y][x] != pattern[i]
      || !matches.emplace(std::make_tuple(x, y, i++)).second)
    return false;
  return MatchPattern(grid, pattern, x - 1, y, i, matches) || MatchPattern(grid, pattern, x + 1, y, i, matches)
      || MatchPattern(grid, pattern, x, y - 1, i, matches) || MatchPattern(grid, pattern, x, y + 1, i, matches);
}

bool IsPatternContainedInGrid(const vector<vector<int>> &grid, const vector<int> &pattern) {
  for (int y = 0; y < grid.size(); ++y)
    for (int x = 0; x < grid.front().size(); ++x) {
      std::unordered_set<std::tuple<int, int, int>> matches;
      if (MatchPattern(grid, pattern, x, y, 0, matches))
        return true;
    }
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"grid", "pattern"};
  return GenericTestMain(args, "is_string_in_matrix.cc",
                         "is_string_in_matrix.tsv", &IsPatternContainedInGrid,
                         DefaultComparator{}, param_names);
}
