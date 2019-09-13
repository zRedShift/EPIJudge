#include <memory>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;
using std::vector;

vector<vector<int>> BinaryTreeDepthOrder(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<vector<int>> vec;
  if (!tree)
    return vec;
  vector<BinaryTreeNode<int> *> curr, next;
  curr.emplace_back(tree.get());
  do {
    vec.emplace_back(vector<int>{});
    for (const auto node: curr) {
      vec.back().emplace_back(node->data);
      if (node->left)
        next.emplace_back(node->left.get());
      if (node->right)
        next.emplace_back(node->right.get());
    }
    curr.clear();
    curr.swap(next);
  } while (!curr.empty());
  return vec;
}

void RecursiveTraversal(const unique_ptr<BinaryTreeNode<int>> &tree, vector<vector<int>> &vec, int depth) {
  if (!tree)
    return;
  if (depth == vec.size())
    vec.emplace_back(vector<int>{});
  vec[depth++].emplace_back(tree->data);
  RecursiveTraversal(tree->left, vec, depth);
  RecursiveTraversal(tree->right, vec, depth);
}

vector<vector<int>> BinaryTreeDepthOrderRecursive(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<vector<int>> vec;
  RecursiveTraversal(tree, vec, 0);
  return vec;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"tree"};
  return GenericTestMain(args, "tree_level_order.cc", "tree_level_order.tsv",
                         &BinaryTreeDepthOrder, DefaultComparator{},
                         param_names);
}
