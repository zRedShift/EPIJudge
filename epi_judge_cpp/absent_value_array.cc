#include <stdexcept>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
using std::invalid_argument;
using std::vector;

int FindMissingElement(vector<int>::const_iterator stream_begin,
                       const vector<int>::const_iterator& stream_end) {
  std::bitset<1 << 16> present;
  while (stream_begin != stream_end)
    present.set(*stream_begin++);
  int i = 0;
  while (present.test(i))
    ++i;
  return i;
}
int FindMissingElementWrapper(const vector<int>& stream) {
  try {
    vector<int> a = stream;
    std::sort(a.begin(), a.end());
    return FindMissingElement(cbegin(stream), cend(stream));
  } catch (invalid_argument&) {
    throw TestFailure("Unexpected no_missing_element exception");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"stream"};
  return GenericTestMain(args, "absent_value_array.cc",
                         "absent_value_array.tsv", &FindMissingElementWrapper,
                         DefaultComparator{}, param_names);
}
