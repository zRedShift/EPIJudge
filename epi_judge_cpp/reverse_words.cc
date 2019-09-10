#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::string;

void ReverseWords(string *s) {
  string::iterator word_start = begin(*s), sep;
  do {
    sep = find(word_start, end(*s), ' ');
    reverse(word_start, sep);
    word_start = sep + 1;
  } while (sep != end(*s));
  reverse(begin(*s), end(*s));
}

string ReverseWordsWrapper(TimedExecutor &executor, string s) {
  executor.Run([&] { ReverseWords(&s); });
  return s;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "s"};
  return GenericTestMain(args, "reverse_words.cc", "reverse_words.tsv",
                         &ReverseWordsWrapper, DefaultComparator{},
                         param_names);
}
