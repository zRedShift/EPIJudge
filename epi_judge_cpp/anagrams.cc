#include <string>
#include <vector>
#include<unordered_map>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

vector<vector<string>> FindAnagrams(const vector<string> &dictionary) {
  std::unordered_map<string, vector<string>> map;
  for (const auto &str: dictionary) {
    string sorted(str);
    std::sort(sorted.begin(), sorted.end());
    map[std::move(sorted)].emplace_back(str);
  }
  vector<vector<string>> result;
  for (auto kv: map)
    if (kv.second.size() > 1)
      result.emplace_back(std::move(kv.second));
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"dictionary"};
  return GenericTestMain(
      args, "anagrams.cc", "anagrams.tsv", &FindAnagrams,
      &UnorderedComparator<std::vector<std::vector<std::string>>>, param_names);
}
