#include <string>
#include "test_framework/generic_test.h"
using std::string;

int SSDecodeColID(const string &col) {
  return accumulate(begin(col), end(col), 0, [](int sum, char c) { return sum * 26 + c - 'A' + 1; });;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"col"};
  return GenericTestMain(args, "spreadsheet_encoding.cc",
                         "spreadsheet_encoding.tsv", &SSDecodeColID,
                         DefaultComparator{}, param_names);
}
