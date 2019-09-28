#include <algorithm>
#include <memory>
#include <stack>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
#include "test_framework/random_sequence_checker.h"
using std::vector;

unique_ptr<BinaryTreeNode<int>> Clone(const unique_ptr<BinaryTreeNode<int>> &tree) {
  return tree ? std::make_unique<BinaryTreeNode<int>>(tree->data, Clone(tree->left), Clone(tree->right)) : nullptr;
}

vector<unique_ptr<BinaryTreeNode<int>>> GenerateAllBinaryTrees(int num_nodes) {
  vector<vector<unique_ptr<BinaryTreeNode<int>>>> results(num_nodes + 1);
  results.front().emplace_back(nullptr);
  for (int i = 1; i <= num_nodes; ++i) {
    auto &result = results[i];
    result.reserve(BinomialCoefficient(2 * i, i) / (i + 1));
    for (int j = 0; j < i; ++j)
      for (const auto &left: results[j])
        for (const auto &right: results[i - j - 1])
          result.emplace_back(std::make_unique<BinaryTreeNode<int>>(0, Clone(left), Clone(right)));
  }
  return std::move(results.back());
}
vector<int> SerializeStructure(const unique_ptr<BinaryTreeNode<int>> &tree) {
  vector<int> result;

  std::stack<BinaryTreeNode<int> *> stack;
  stack.push(tree.get());
  while (!stack.empty()) {
    auto p = stack.top();
    stack.pop();
    result.push_back(p != nullptr);
    if (p) {
      stack.push(p->left.get());
      stack.push(p->right.get());
    }
  }
  return result;
}

vector<vector<int>> GenerateAllBinaryTreesWrapper(TimedExecutor &executor,
                                                  int num_nodes) {
  auto result = executor.Run([&] { return GenerateAllBinaryTrees(num_nodes); });

  vector<vector<int>> serialized;
  for (auto &x : result) {
    serialized.push_back(SerializeStructure(x));
  }
  std::sort(begin(serialized), end(serialized));
  return serialized;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "num_nodes"};
  return GenericTestMain(args, "enumerate_trees.cc", "enumerate_trees.tsv",
                         &GenerateAllBinaryTreesWrapper, DefaultComparator{},
                         param_names);
}
