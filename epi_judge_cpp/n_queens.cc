#include <algorithm>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void NQueensSolver(const int n, vector<int> &configuration, vector<vector<int>> &configurations) {
  if (configuration.size() == n) {
    configurations.emplace_back(configuration);
    return;
  }
  vector<bool> possible(n, true);
  int prev = configuration.size();
  for (const auto x: configuration) {
    possible[x] = false;
    if (const int left = x - prev; left > -1)
      possible[left] = false;
    if (const int right = x + prev; right < n)
      possible[right] = false;
    --prev;
  }
  for (int i = 0; i < n; ++i)
    if (possible[i]) {
      configuration.push_back(i);
      NQueensSolver(n, configuration, configurations);
      configuration.pop_back();
    }
}

vector<vector<int>> NQueens(const int n) {
  vector<vector<int>> result;
  vector<int> configuration;
  configuration.reserve(n);
  NQueensSolver(n, configuration, result);
  return result;
}
bool Comp(vector<vector<int>> &a, vector<vector<int>> &b) {
  std::sort(std::begin(a), std::end(a));
  std::sort(std::begin(b), std::end(b));
  return a == b;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n"};
  return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                         param_names);
}
