#include <algorithm>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::max;
using std::min;
using std::string;
using std::vector;

bool IsValid(const string &s, int i, int j) {
  switch (j) {
    case 1:return true;
    case 2:return s[i] != '0';
    default:return s[i] != '0' && (s[i] - '0') * 100 + (s[i + 1] - '0') * 10 + s[i + 2] - '0' < 256;
  }
}

vector<string> GetValidIpAddress(const string &s) {
  vector<string> ips;
  for (int i = max(1, (int) size(s) - 9); i < min(4, (int) size(s) - 2); ++i)
    if (IsValid(s, 0, i))
      for (int j = max(i + 1, (int) size(s) - 6); j < min(i + 4, (int) size(s) - 1); ++j)
        if (IsValid(s, i, j - i))
          for (int k = max(j + 1, (int) size(s) - 3); k < min(j + 4, (int) size(s)); ++k)
            if (IsValid(s, j, k - j) && IsValid(s, k, (int) size(s) - k)) {
              string ip;
              ip.reserve(size(s) + 3), ip.append(s, 0, i).push_back('.');
              ip.append(s, i, j - i).push_back('.');
              ip.append(s, j, k - j).push_back('.');
              ips.emplace_back(std::move(ip.append(s, k)));
            }
  return ips;
}

bool comp(vector<string> expected, vector<string> result) {
  std::sort(begin(expected), end(expected));
  std::sort(begin(result), end(result));
  return expected == result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"s"};
  return GenericTestMain(args, "valid_ip_addresses.cc",
                         "valid_ip_addresses.tsv", &GetValidIpAddress, &comp,
                         param_names);
}
