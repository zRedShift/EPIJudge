#include <unordered_map>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::vector;

struct Subarray {
  // Represent subarray by starting and ending indices, inclusive.
  int start, end;
};
Subarray FindSmallestSequentiallyCoveringSubset(const vector<string> &paragraph, const vector<string> &keywords) {
  int dict_size = keywords.size();
  std::unordered_map<string, int> key_to_i(dict_size);
  for (int i = 0; i < dict_size; ++i)
    key_to_i.emplace(keywords[i], i);
  std::vector<int> last_position(dict_size + 1), smallest_subarray(dict_size + 1, -1);
  smallest_subarray[0] = 0;
  Subarray result{0, std::numeric_limits<int>::max()};
  int subarray_size = std::numeric_limits<int>::max(), subarray_end = -1;
  for (int &i = last_position[0]; i < paragraph.size(); ++i) {
    auto found = key_to_i.find(paragraph[i]);
    if (found == key_to_i.end())
      continue;
    int key = found->second + 1;
    last_position[key] = i;
    if (smallest_subarray[key - 1] != -1) {
      smallest_subarray[key] = smallest_subarray[key - 1] + i - last_position[key - 1];
      if (key == dict_size && smallest_subarray.back() < subarray_size)
        subarray_size = smallest_subarray.back(), subarray_end = i;
    }
  }
  return {subarray_end - subarray_size, subarray_end};
}

int FindSmallestSequentiallyCoveringSubsetWrapper(
    TimedExecutor &executor, const vector<string> &paragraph,
    const vector<string> &keywords) {
  auto result = executor.Run([&] {
    return FindSmallestSequentiallyCoveringSubset(paragraph, keywords);
  });

  int kw_idx = 0;
  if (result.start < 0) {
    throw TestFailure("Subarray start index is negative");
  }
  int para_idx = result.start;

  while (kw_idx < keywords.size()) {
    if (para_idx >= paragraph.size()) {
      throw TestFailure("Not all keywords are in the generated subarray");
    }
    if (para_idx >= paragraph.size()) {
      throw TestFailure("Subarray end index exceeds array size");
    }
    if (paragraph[para_idx] == keywords[kw_idx]) {
      kw_idx++;
    }
    para_idx++;
  }
  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "paragraph", "keywords"};
  return GenericTestMain(args, "smallest_subarray_covering_all_values.cc",
                         "smallest_subarray_covering_all_values.tsv",
                         &FindSmallestSequentiallyCoveringSubsetWrapper,
                         DefaultComparator{}, param_names);
}
