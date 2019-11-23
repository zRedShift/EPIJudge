#include <iterator>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;
using Iter = vector<string>::const_iterator;

vector<string> SearchFrequentItems(int k, Iter stream_begin, const Iter stream_end) {
  std::unordered_map<std::string, int> m;
  auto iter = stream_begin;
  int threshold = 0;
  while (iter != stream_end)
    if (++threshold; m.size() < k)
      ++m[*iter++];
    else if (auto found = m.find(*iter++); found != m.end())
      ++found->second;
    else
      for (auto p = m.begin(); p != m.end(); --p->second ? ++p : m.erase(p++)) continue;
  for (auto &p : m)
    p.second = 0;
  while (stream_begin != stream_end)
    if (auto found = m.find(*stream_begin++); found != m.end())
      ++found->second;
  vector<string> result;
  threshold /= k;
  for (const auto &p : m)
    if (p.second > threshold)
      result.emplace_back(p.first);
  return result;
}

vector<string> SearchFrequentItemsAlt(int k, Iter stream_begin, const Iter stream_end) {
  std::unordered_map<std::string_view, int> m;
  int threshold{static_cast<int>(std::distance(stream_begin, stream_end) / k)};
  while (stream_begin != stream_end)
    ++m[*stream_begin++];
  vector<string> result;
  for (const auto &p : m)
    if (p.second > threshold)
      result.emplace_back(p.first);
  return result;
}

vector<string> SearchFrequentItemsWrapper(int k, vector<string> &stream) {
  return SearchFrequentItems(k, begin(stream), end(stream));
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"k", "stream"};
  return GenericTestMain(
      args, "search_frequent_items.cc", "search_frequent_items.tsv",
      &SearchFrequentItemsWrapper,
      &UnorderedComparator<std::vector<std::string>>, param_names);
}
