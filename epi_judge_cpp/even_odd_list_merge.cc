#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> EvenOddMerge(const shared_ptr<ListNode<int>> &L) {
  auto dummy_head = make_shared<ListNode<int>>(ListNode<int>{0, L}), first = dummy_head, second = dummy_head;
  bool parity = false;
  while (second->next) {
    first = second;
    second = second->next;
    first->next = second->next;
    parity = !parity;
  }
  parity ? second->next = dummy_head->next : first->next = dummy_head->next;
  return L;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L"};
  return GenericTestMain(args, "even_odd_list_merge.cc",
                         "even_odd_list_merge.tsv", &EvenOddMerge,
                         DefaultComparator{}, param_names);
}
