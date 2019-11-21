#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int CalculateBonus(const vector<int> &productivity) {
  if (productivity.size() < 2)
    return productivity.size();
  vector<int> vec(productivity.size()), ticket(productivity.size(), 1);
  std::iota(vec.begin(), vec.end(), 0);
  auto cmp = [&productivity](const auto &l, const auto &r) { return productivity[l] > productivity[r]; };
  std::priority_queue<int, vector<int>, decltype(cmp)> min_heap(cmp, std::move(vec));
  do {
    const int i = min_heap.top();
    min_heap.pop();
    if (i == 0) {
      if (productivity[0] >= productivity[1]) ticket[0] = ticket[1] + (productivity[0] != productivity[1]);
    } else if (i == productivity.size() - 1) {
      if (productivity[i] >= productivity[i - 1]) ticket[i] = ticket[i - 1] + (productivity[i] != productivity[i - 1]);
    } else if (productivity[i] >= productivity[i - 1] && productivity[i] >= productivity[i + 1])
      ticket[i] = std::max(ticket[i - 1], ticket[i + 1])
          + (productivity[i] != std::max(productivity[i - 1], productivity[i + 1]));
    else if (productivity[i] >= productivity[i - 1] || productivity[i] >= productivity[i + 1])
      ticket[i] = std::min(ticket[i - 1], ticket[i + 1])
          + (productivity[i] != std::min(productivity[i - 1], productivity[i + 1]));
  } while (!min_heap.empty());
  return std::accumulate(ticket.begin(), ticket.end(), 0);
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"productivity"};
  return GenericTestMain(args, "bonus.cc", "bonus.tsv", &CalculateBonus,
                         DefaultComparator{}, param_names);
}
