#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int FindKthLargestUnknownLength(vector<int>::const_iterator stream_begin,
                                const vector<int>::const_iterator &stream_end,
                                int k) {
  auto curr = std::min(stream_begin + 2 * k, stream_end);
  vector<int> A(stream_begin, curr);
  auto result = A.end() - k;
  std::nth_element(A.begin(), result, A.end());
  for (; curr + k <= stream_end; curr += k)
    std::copy_n(curr, k, A.begin()), std::nth_element(A.begin(), result, A.end());
  if (curr != stream_end)
    std::copy(curr, stream_end, A.begin()), std::nth_element(A.begin(), result, A.end());
  return *result;
}
int FindKthLargestUnknownLengthWrapper(const vector<int> &stream, int k) {
  return FindKthLargestUnknownLength(stream.cbegin(), stream.cend(), k);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream", "k"};
  return GenericTestMain(args, "kth_largest_element_in_long_array.cc",
                         "kth_largest_element_in_long_array.tsv",
                         &FindKthLargestUnknownLengthWrapper,
                         DefaultComparator{}, param_names);
}
