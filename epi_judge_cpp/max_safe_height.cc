#include "test_framework/generic_test.h"

int GetHeightAlt(int cases, int drops) {
  if (cases < 3)
    return cases * drops + (cases == 2) * drops * (drops - 3) / 2;
  if (cases >= drops - 3)
    return (1 << drops) - 1 - (cases < drops) - (cases < drops - 1) * drops
        - (cases < drops - 2) * drops * (drops - 1) / 2;
  bool sub = 2 * cases > drops;
  cases = sub ? drops - cases - 1 : cases;
  std::vector<int> row(cases + 1, 1), prev = row;
  for (int i = 1; i < cases; ++i) {
    for (int j = 1; j < i; ++j)
      row[j] = prev[j] + prev[j - 1];
    std::swap(row, prev);
  }
  for (int i = cases; i < drops; ++i) {
    for (int j = 1; j < row.size(); ++j)
      row[j] = prev[j] + prev[j - 1];
    std::swap(row, prev);
  }
  int sum = prev.back() + 2 * std::accumulate(prev.begin() + 1, prev.end() - 1, 0);
  return sub ? (1 << drops) - 2 - sum : sum;
}

int GetHeight(int cases, int drops) {
  if (cases >= drops)
    return (1 << drops) - 1;
  int sum = 0, i = 1;
  for (size_t prev = 1; i <= cases; ++i)
    sum += prev = prev * (drops - i + 1) / i;
  return sum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"cases", "drops"};
  return GenericTestMain(args, "max_safe_height.cc", "max_safe_height.tsv",
                         &GetHeight, DefaultComparator{}, param_names);
}
