#include <set>
#include <stdexcept>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
void Advance(shared_ptr<ListNode<int>> &l, int i) {
  while (i--)
    l = l->next;
}
shared_ptr<ListNode<int>> HasCycle(const shared_ptr<ListNode<int>> &head, int &len, int &cycle) {
  int i = 0;
  cycle = 0;
  auto fast = head, slow = head;
  while (fast && fast->next) {
    fast = fast->next->next;
    if (slow != fast) {
      ++i;
      if (slow = slow->next; slow != fast)
        continue;
    }
    do {
      slow = slow->next;
      ++cycle;
    } while (slow != fast);
    fast = head;
    if (i < cycle) {
      Advance(slow, cycle - i);
      len = cycle;
    } else {
      Advance(fast, i - cycle);
      len = i;
    }
    while ((fast = fast->next) != (slow = slow->next))
      ++len;
    return fast;
  }
  return nullptr;
}
void Length(shared_ptr<ListNode<int>> &l, int &i) {
  for (i = 0; l->next; ++i, l = l->next);
}
shared_ptr<ListNode<int>> OverlappingNoCycleLists(
    shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
  if (!l0 || !l1)
    return nullptr;
  auto m0 = l0, m1 = l1;
  int i, j;
  Length(l0, i), Length(l1, j);
  if (l0 != l1)
    return nullptr;
  i < j ? Advance(m1, j - i) : Advance(m0, i - j);
  while ((m0 = m0->next) != (m1 = m1->next));
  return m0;
}
shared_ptr<ListNode<int>> OverlappingLists(shared_ptr<ListNode<int>> l0,
                                           shared_ptr<ListNode<int>> l1) {
  int len0, cyc0, len1, cyc1;
  auto c0 = HasCycle(l0, len0, cyc0), c1 = HasCycle(l1, len1, cyc1);
  if (!cyc0 && !cyc1)
    return OverlappingNoCycleLists(l0, l1);
  if (cyc0 != cyc1)
    return nullptr;
  while (cyc1-- && c1 != c0)
    c1 = c1->next;
  if (c1 != c0)
    return nullptr;
  int diff = abs(len1 - len0);
  Advance(len0 < len1 ? l1 : l0, diff);
  while (diff-- && l1 != l0) {
    l1 = l1->next;
    l0 = l0->next;
  }
  return l1 == l0 ? l0 : c0;
}
void OverlappingListsWrapper(TimedExecutor &executor,
                             shared_ptr<ListNode<int>> l0,
                             shared_ptr<ListNode<int>> l1,
                             shared_ptr<ListNode<int>> common, int cycle0,
                             int cycle1) {
  if (common) {
    if (!l0) {
      l0 = common;
    } else {
      auto it = l0;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }

    if (!l1) {
      l1 = common;
    } else {
      auto it = l1;
      while (it->next) {
        it = it->next;
      }
      it->next = common;
    }
  }

  if (cycle0 != -1 && l0) {
    auto last = l0;
    while (last->next) {
      last = last->next;
    }
    auto it = l0;
    while (cycle0-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  if (cycle1 != -1 && l1) {
    auto last = l1;
    while (last->next) {
      last = last->next;
    }
    auto it = l1;
    while (cycle1-- > 0) {
      if (!it) {
        throw std::runtime_error("Invalid input data");
      }
      it = it->next;
    }
    last->next = it;
  }

  std::set<shared_ptr<ListNode<int>>> common_nodes;
  auto it = common;
  while (it && common_nodes.count(it) == 0) {
    common_nodes.insert(it);
    it = it->next;
  }

  auto result = executor.Run([&] { return OverlappingLists(l0, l1); });

  if (!((common_nodes.empty() && result == nullptr) ||
      common_nodes.count(result) > 0)) {
    throw TestFailure("Invalid result");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "l0", "l1",
                                       "common", "cycle0", "cycle1"};
  return GenericTestMain(args, "do_lists_overlap.cc", "do_lists_overlap.tsv",
                         &OverlappingListsWrapper, DefaultComparator{},
                         param_names);
}
