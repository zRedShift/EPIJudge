#include <list>
#include <string>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

struct Subarray {
  bool operator<(const Subarray &rhs) const {
    return end - start < rhs.end - rhs.start;
  }
  int start, end;
};

Subarray FindSmallestSubarrayCoveringSet(
    const vector<string> &paragraph, const unordered_set<string> &keywords) {
  Subarray result{0, std::numeric_limits<int>::max()};
  std::unordered_map<string, std::list<int>::iterator> last_appearance(keywords.size());
  std::list<int> ordered_appearance;
  for (int i = 0; i < paragraph.size(); ++i)
    if (keywords.find(paragraph[i]) != keywords.end()) {
      ordered_appearance.push_front(i);
      auto prev = last_appearance.find(paragraph[i]);
      if (prev == last_appearance.end())
        last_appearance.emplace(paragraph[i], ordered_appearance.begin());
      else
        ordered_appearance.erase(prev->second), prev->second = ordered_appearance.begin();
      if (ordered_appearance.size() == keywords.size())
        result = std::min(result, Subarray{ordered_appearance.back(), ordered_appearance.front()});
    }
  return result;
}
int FindSmallestSubarrayCoveringSetWrapper(
    TimedExecutor &executor, const vector<string> &paragraph,
    const unordered_set<string> &keywords) {
  unordered_set<string> copy = keywords;

  auto result = executor.Run(
      [&] { return FindSmallestSubarrayCoveringSet(paragraph, keywords); });

  if (result.start < 0 || result.start >= paragraph.size() || result.end < 0 ||
      result.end >= paragraph.size() || result.start > result.end) {
    throw TestFailure("Index out of range");
  }

  for (int i = result.start; i <= result.end; i++) {
    copy.erase(paragraph[i]);
  }

  if (!copy.empty()) {
    throw TestFailure("Not all keywords are in the range");
  }

  return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "paragraph", "keywords"};
  return GenericTestMain(args, "smallest_subarray_covering_set.cc",
                         "smallest_subarray_covering_set.tsv",
                         &FindSmallestSubarrayCoveringSetWrapper,
                         DefaultComparator{}, param_names);
}
