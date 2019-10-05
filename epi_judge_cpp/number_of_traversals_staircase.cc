#include "test_framework/generic_test.h"

int NumberOfWaysToTop(int top, int maximum_step) {
  if (maximum_step < 2)
    return maximum_step;
  if (maximum_step >= top--)
    return 1 << top;
  std::vector<int> circular_queue;
  circular_queue.reserve(maximum_step);
  int sum = 1;
  circular_queue.emplace_back(sum);
  std::generate_n(std::back_inserter(circular_queue), maximum_step - 1, [&sum] { return sum *= 2; });
  sum = sum * 2 - 1;
  for (int i = maximum_step; i < top; ++i) {
    int &curr = circular_queue[i % maximum_step];
    std::swap(curr, sum);
    sum = curr * 2 - sum;
  }
  return sum;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"top", "maximum_step"};
  return GenericTestMain(args, "number_of_traversals_staircase.cc",
                         "number_of_traversals_staircase.tsv",
                         &NumberOfWaysToTop, DefaultComparator{}, param_names);
}
