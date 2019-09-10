#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"
using std::shared_ptr;

shared_ptr<ListNode<int>> RemoveDuplicates(const shared_ptr<ListNode<int>> &L) {
  auto first = L;
  while (first) {
    auto second = first->next;
    while (second && second->data == first->data)
      second = second->next;
    first->next = second;
    first = second;
  }
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "remove_duplicates_from_sorted_list.cc",
                         "remove_duplicates_from_sorted_list.tsv",
                         &RemoveDuplicates, DefaultComparator{}, param_names);
}
