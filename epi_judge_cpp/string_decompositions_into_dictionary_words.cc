#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::string_view;
using std::min;
using std::sort;
using std::unordered_map;
using std::vector;

void UpdateCount(unordered_map<string_view, int> &live_count, const string_view &curr, int quantity) {
  auto temp = live_count.emplace(curr, quantity);
  if (!temp.second && !(temp.first->second += quantity))
    live_count.erase(temp.first);
}

vector<int> FindAllSubstrings(const string &s, const vector<string> &words) {
  const int kLen = words[0].size(), kSize = words.size() * kLen, kOff = min(kLen, int(s.size() - kSize + 1));
  if (s.size() < kSize)
    return {};
  unordered_map<string_view, int> counts(words.size()), live_count;
  vector<int> result;
  for (const auto &word : words)
    ++counts[string_view(word)];
  for (int off = 0; off < kOff; ++off) {
    bool valid = false;
    int i = off, j = i + kSize;
    live_count = counts;
    while (true) {
      if (!valid && j <= s.size()) {
        bool touched = false;
        do {
          if (touched)
            live_count = counts, touched = false;
          for (string_view curr(&s[i], kLen); counts.count(curr) && i < j; curr = string_view(&s[i += kLen], kLen))
            UpdateCount(live_count, curr, -1), touched = true;
          if (i == j) {
            i -= kSize;
            if (live_count.empty())
              result.emplace_back(i);
            valid = true;
            break;
          } else
            i += kLen, j = i + kSize;
        } while (j <= s.size());
      }
      if (j < s.size())
        do {
          string_view prev(&s[i], kLen), next(&s[j], kLen);
          if (!counts.count(next)) {
            i = j + kLen, j = i + kSize, valid = false, live_count = counts;
            break;
          }
          UpdateCount(live_count, prev, 1);
          UpdateCount(live_count, next, -1);
          i += kLen, j += kLen;
          if (live_count.empty())
            result.emplace_back(i);
        } while (j < s.size());
      else
        break;
    }
  }
  sort(result.begin(), result.end());
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s", "words"};
  return GenericTestMain(args, "string_decompositions_into_dictionary_words.cc",
                         "string_decompositions_into_dictionary_words.tsv",
                         &FindAllSubstrings, DefaultComparator{}, param_names);
}
