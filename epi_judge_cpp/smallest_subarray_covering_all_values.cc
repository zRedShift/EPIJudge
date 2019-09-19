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
  bool operator<(const Subarray &rhs) const {
    return end - start < rhs.end - rhs.start;
  }
  int start, end;
};

Subarray FindSmallestSequentiallyCoveringSubset(
    const vector<string> &paragraph, const vector<string> &keywords) {
  std::unordered_map<string, int> keyword_to_position(keywords.size());
  for (int i = 0; i < keywords.size(); ++i)
    keyword_to_position[keywords[i]] = i;
  vector<vector<int>> positions(keywords.size());
  for (int i = 0; i < paragraph.size(); ++i)
    if (auto pos = keyword_to_position.find(paragraph[i]); pos != keyword_to_position.end())
      positions[pos->second].push_back(i);
  vector<vector<int>::iterator> subset;
  subset.reserve(keywords.size());
  for (auto &pos: positions)
    subset.emplace_back(pos.begin());
  Subarray result{positions.front().front(), positions.back().back()};
  while (subset.front() != positions.front().end()) {
    for (int i = 1; i < keywords.size(); ++i) {
      subset[i] = std::upper_bound(subset[i], positions[i].end(), *subset[i - 1]);
      if (subset[i] == positions[i].end())
        return result;
    }
    for (int i = keywords.size() - 2; i >= 0; --i)
      subset[i] = std::upper_bound(subset[i], positions[i].end(), *subset[i + 1]) - 1;
    result = std::min(result, {*subset.front()++, *subset.back()});
  }
  return result;
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
