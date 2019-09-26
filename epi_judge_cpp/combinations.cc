#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
using std::vector;
bool IncrementCombination(const vector<int>::iterator &left, const vector<int>::iterator &right, int n) {
  for (auto p = right - 1; p != left; --p, --n)
    if (*p != n) {
      n = ++*p++ + 1;
      while (p != right)
        *p++ = n++;
      return true;
    }
  return false;
}

vector<vector<int>> Combinations(int n, int k) {
  if (!k)
    return {{}};
  vector<vector<int>> result;
  result.reserve(BinomialCoefficient(n, k));
  vector<int> combination(k);
  auto left = combination.begin(), right = combination.end();
  std::iota(left--, right, 1);
  do {
    result.emplace_back(combination);
  } while (IncrementCombination(left, right, n));
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "combinations.cc", "combinations.tsv", &Combinations,
      &UnorderedComparator<std::vector<std::vector<int>>>, param_names);
}
