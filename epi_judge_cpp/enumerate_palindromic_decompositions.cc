#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

bool IsPalindrome(string::const_iterator start, string::const_iterator end) {
  while (start < end)
    if (*start++ != *end--)
      return false;
  return true;
}

void Decompose(const string::const_iterator &start,
               const string::const_iterator &end,
               vector<string> &current,
               vector<vector<string>> &output) {
  if (start == end) {
    output.emplace_back(current);
    return;
  }
  for (auto p = start; p != end;)
    if (IsPalindrome(start, p++)) {
      current.emplace_back(start, p);
      Decompose(p, end, current, output);
      current.pop_back();
    }
}

vector<vector<string>> PalindromeDecompositions(const string &input) {
  vector<vector<string>> result;
  vector<string> current;
  current.reserve(input.size());
  Decompose(input.cbegin(), input.cend(), current, result);
  return result;
}
bool Comp(vector<vector<string>> expected, vector<vector<string>> result) {
  std::sort(begin(expected), end(expected));
  std::sort(begin(result), end(result));
  return expected == result;
};

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"input"};
  return GenericTestMain(args, "enumerate_palindromic_decompositions.cc",
                         "enumerate_palindromic_decompositions.tsv",
                         &PalindromeDecompositions, &Comp, param_names);
}
