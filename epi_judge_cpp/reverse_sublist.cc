#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {
  auto dummy_head = make_shared<ListNode<int>>(ListNode<int>{0, move(L)});
  auto before = dummy_head;
  for (int i = 1; i < start; ++i)
    before = before->next;
  auto first = before->next;
  for (int i = start; i < finish; ++i) {
    auto temp = first->next;
    first->next = temp-> next;
    temp-> next = before->next;
    before->next = temp;
  }
  return dummy_head->next;
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"L", "start", "finish"};
  return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                         &ReverseSublist, DefaultComparator{}, param_names);
}
