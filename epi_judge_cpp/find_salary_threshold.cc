#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

double FindSalaryCap(int target_payroll, vector<int> current_salaries) {
  std::sort(current_salaries.begin(), current_salaries.end());
  int n = current_salaries.size(), sum = 0;
  for (const auto salary: current_salaries) {
    if (n * salary >= target_payroll - sum)
      return double(target_payroll - sum) / n;
    sum += salary, --n;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"target_payroll", "current_salaries"};
  return GenericTestMain(args, "find_salary_threshold.cc",
                         "find_salary_threshold.tsv", &FindSalaryCap,
                         DefaultComparator{}, param_names);
}
