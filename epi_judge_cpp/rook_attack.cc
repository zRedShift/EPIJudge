#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

void RookAttack(vector<vector<int>> *A_ptr) {
  auto &A = *A_ptr;
  int m = A.size(), n = A[0].size();
  bool col = A[0][0], row = std::find(A[0].begin(), A[0].end(), 0) == A[0].end();
  for (int i = 1; i < m; ++i) {
    col &= A[i][0] != 0;
    for (int j = 1; j < n; ++j)
      if (!A[i][j])
        A[i][0] = A[0][j] = 0;
  }
  A[0][0] *= col;
  for (auto p = A.begin() + 1; p != A.end(); ++p)
    if (!p->front())
      std::fill(p->begin() + 1, p->end(), 0);
  for (int j = 0; j < n; ++j)
    if (!A[0][j])
      std::for_each(A.begin() + 1, A.end(), [j](auto &row) { row[j] = 0; });
  if (!row)
    std::fill(A[0].begin(), A[0].end(), 0);
}

vector<vector<int>> RookAttackWrapper(vector<vector<int>> A) {
  RookAttack(&A);
  return A;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"A"};
  return GenericTestMain(args, "rook_attack.cc", "rook_attack.tsv",
                         &RookAttackWrapper, DefaultComparator{}, param_names);
}
