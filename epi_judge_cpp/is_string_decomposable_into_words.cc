#include <string>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

vector<string> DecomposeIntoDictionaryWords(const string &domain, const unordered_set<string> &dictionary) {
  if (dictionary.empty())
    return {};
  vector<size_t> last(domain.size() + 1);
  size_t max_word = std::max_element(dictionary.begin(), dictionary.end(), [](const string &a, const string &b) {
    return a.size() < b.size();
  })->size(), pos = 0, len = 0, result_size = 0, max_len = std::min(max_word, domain.size());
  string temp;
  temp.reserve(max_word);
  do {
    while (++len <= max_len)
      if (!last[pos + len] && dictionary.count(temp.assign(domain, pos, len))) {
        pos += len, last[pos] = len, result_size += 1, len = 0, max_len = std::min(max_word, domain.size() - pos);
        if (pos == domain.size()) {
          vector<string> result(result_size);
          do {
            len = last[pos], pos -= len;
            result[--result_size] = domain.substr(pos, len);
          } while (result_size);
          return result;
        }
      }
    len = last[pos], pos -= len, result_size -= 1, max_len = std::min(max_word, domain.size() - pos);
  } while (result_size != -1);
  return {};
}

void DecomposeIntoDictionaryWordsWrapper(
    TimedExecutor &executor, const string &domain,
    const unordered_set<string> &dictionary, bool decomposable) {
  vector<string> result = executor.Run(
      [&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
  if (!decomposable) {
    if (!result.empty()) {
      throw TestFailure("domain is not decomposable");
    }
    return;
  }

  if (std::any_of(std::begin(result), std::end(result),
                  [&](const std::string &s) { return !dictionary.count(s); })) {
    throw TestFailure("Result uses words not in dictionary");
  }

  if (std::accumulate(std::begin(result), std::end(result), string()) !=
      domain) {
    throw TestFailure("Result is not composed into domain");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "domain", "dictionary",
                                       "decomposable"};
  return GenericTestMain(args, "is_string_decomposable_into_words.cc",
                         "is_string_decomposable_into_words.tsv",
                         &DecomposeIntoDictionaryWordsWrapper,
                         DefaultComparator{}, param_names);
}
