#include <string>
#include "test_framework/generic_test.h"
using std::string;

// Returns the index of the first character of the substring if found, -1
// otherwise.
int RabinKarp(const string &t, const string &s) {
  int diff = t.size() - s.size();
  if (diff < 0)
    return -1;
  const unsigned kBase = 256, kModulus = 32749;
  unsigned s_hash = 0, t_hash = 0, power = 3454;
  for (auto i = 0; i < s.size(); ++i) {
    power = power * kBase % kModulus;
    s_hash = (s_hash * kBase + s[i]) % kModulus;
    t_hash = (t_hash * kBase + t[i]) % kModulus;
  }
  for (int i = 0; i < diff; ++i) {
    if (t_hash == s_hash && t.compare(i, s.size(), s) == 0)
      return i;
    t_hash = ((t_hash + kModulus - t[i] * power % kModulus) * kBase + t[i + s.size()]) % kModulus;
  }
  if (t_hash == s_hash && t.compare(diff, s.size(), s) == 0)
    return diff;
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"t", "s"};
  return GenericTestMain(args, "substring_match.cc", "substring_match.tsv",
                         &RabinKarp, DefaultComparator{}, param_names);
}
