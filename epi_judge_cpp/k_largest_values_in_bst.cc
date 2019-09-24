#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;
void ReverseInorderTraversal(const unique_ptr<BstNode<int>> &tree, int k, vector<int> &vec) {
  if (tree && vec.size() < k) {
    ReverseInorderTraversal(tree->right, k, vec);
    if (vec.size() < k)
      vec.emplace_back(tree->data), ReverseInorderTraversal(tree->left, k, vec);
  }
}

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>> &tree, int k) {
  vector<int> result;
  result.reserve(k);
  ReverseInorderTraversal(tree, k, result);
  return result;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree", "k"};
  return GenericTestMain(args, "k_largest_values_in_bst.cc",
                         "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                         &UnorderedComparator<std::vector<int>>, param_names);
}
