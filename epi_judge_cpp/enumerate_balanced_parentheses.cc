#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/random_sequence_checker.h"
using std::string;
using std::vector;
void BalancedParentheses(string &configuration, const int i, const int open, vector<string> &result) {
  if (open + i == configuration.size()) {
    configuration.replace(i, string::npos, open, ')');
    result.emplace_back(configuration);
    return;
  }
  configuration[i] = '(';
  BalancedParentheses(configuration, i + 1, open + 1, result);
  if (open) {
    configuration[i] = ')';
    BalancedParentheses(configuration, i + 1, open - 1, result);
  }
}

vector<string> GenerateBalancedParentheses(int num_pairs) {
  const int str_len = 2 * num_pairs;
  vector<string> result;
  result.reserve(BinomialCoefficient(str_len, num_pairs) / (num_pairs + 1));
  string configuration(str_len, '\0');
  BalancedParentheses(configuration, 0, 0, result);
  return result;
}

vector<string> GenerateBalancedParenthesesNonRecursive(int num_pairs) {
  static vector<vector<string>> results{{""}, {"()"}};
  string configuration;
  configuration.reserve(2 * num_pairs);
  configuration += '(';
  for (int i = results.size(); i <= num_pairs; ++i) {
    vector<string> &result = results.emplace_back();
    result.reserve(BinomialCoefficient(2 * i, i) / (i + 1));
    for (int j = 0; j < i; ++j)
      for (const auto &str1: results[j])
        for (const auto &str2: results[i - j - 1]) {
          configuration.replace(1, string::npos, str1);
          configuration += ')';
          configuration += str2;
          result.emplace_back(configuration);
        }
  }
  return results[num_pairs];
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"num_pairs"};
  return GenericTestMain(args, "enumerate_balanced_parentheses.cc",
                         "enumerate_balanced_parentheses.tsv",
                         &GenerateBalancedParentheses,
                         &UnorderedComparator<vector<string>>, param_names);
}
