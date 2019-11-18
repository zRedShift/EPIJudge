#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool IsArbitrageExist(vector<vector<double>> graph) {
  for (auto &row: graph)
    for (auto &x : row)
      x = log2(x);
  bool changed;
  do {
    changed = false;
    for (int i = 1; i < graph.size(); ++i)
      for (int j = 0; j < graph.size(); ++j)
        if (graph[0][i] + graph[i][j] > graph[0][j])
          graph[0][j] = graph[0][i] + graph[i][j], changed = true;
  } while (!graph[0][0] && changed);
  return changed;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"graph"};
  return GenericTestMain(args, "arbitrage.cc", "arbitrage.tsv",
                         &IsArbitrageExist, DefaultComparator{}, param_names);
}
