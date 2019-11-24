#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool ExpressionSynthesisHelper(const vector<vector<int>> &numbers, int position, int prev, int curr, int target) {
  if (position == numbers.size())
    return prev + curr == target;
  if (prev + curr > target)
    return numbers[position][0] ? false : ExpressionSynthesisHelper(numbers, position + 1, prev, 0, target);
  int size = numbers[position].size();
  for (int i = 0, num = numbers[position][0]; i < size; num = numbers[position][++i])
    if (ExpressionSynthesisHelper(numbers, position + i + 1, prev + curr, num, target)
        || ExpressionSynthesisHelper(numbers, position + i + 1, prev, curr * num, target))
      return true;
  return false;
}

bool ExpressionSynthesis(const vector<int> &digits, int target) {
  vector<vector<int>> numbers(digits.size());
  for (int i = 0; i < digits.size(); ++i)
    for (int j = i, num = 0; j < digits.size(); ++j)
      numbers[i].emplace_back(num = (num * 10) + digits[j]);
  for (int i = 0, num = numbers.front()[0]; i < digits.size() && num <= target; num = numbers.front()[++i])
    if (ExpressionSynthesisHelper(numbers, i + 1, 0, num, target))
      return true;
  return false;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"digits", "target"};
  return GenericTestMain(args, "insert_operators_in_string.cc",
                         "insert_operators_in_string.tsv", &ExpressionSynthesis,
                         DefaultComparator{}, param_names);
}
