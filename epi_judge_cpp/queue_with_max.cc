#include <stdexcept>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
using std::length_error;

class QueueWithMax {
 public:
  void Enqueue(int x) {
    queue_.push(x);
    if (max_.empty()) {
      max_.push_back(x);
      return;
    }
    if (x > max_.front()) {
      max_ = {x};
      return;
    }
    while (x > max_.back())
      max_.pop_back();
    max_.push_back(x);
  }
  int Dequeue() {
    int x = queue_.front();
    queue_.pop();
    if (x == max_.front())
      max_.pop_front();
    return x;
  }
  [[nodiscard]] int Max() const { return max_.front(); }
 private:
  std::queue<int> queue_;
  std::deque<int> max_;
};
struct QueueOp {
  enum { kConstruct, kDequeue, kEnqueue, kMax } op;
  int argument;

  QueueOp(const std::string &op_string, int arg) : argument(arg) {
    if (op_string == "QueueWithMax") {
      op = kConstruct;
    } else if (op_string == "dequeue") {
      op = kDequeue;
    } else if (op_string == "enqueue") {
      op = kEnqueue;
    } else if (op_string == "max") {
      op = kMax;
    } else {
      throw std::runtime_error("Unsupported queue operation: " + op_string);
    }
  }
};

template<>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp> &ops) {
  try {
    QueueWithMax q;
    for (auto &x : ops) {
      switch (x.op) {
        case QueueOp::kConstruct:break;
        case QueueOp::kDequeue: {
          int result = q.Dequeue();
          if (result != x.argument) {
            throw TestFailure("Dequeue: expected " +
                std::to_string(x.argument) + ", got " +
                std::to_string(result));
          }
        }
          break;
        case QueueOp::kEnqueue:q.Enqueue(x.argument);
          break;
        case QueueOp::kMax: {
          int s = q.Max();
          if (s != x.argument) {
            throw TestFailure("Max: expected " + std::to_string(x.argument) +
                ", got " + std::to_string(s));
          }
        }
          break;
      }
    }
  } catch (const length_error &) {
    throw TestFailure("Unexpected length_error exception");
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "queue_with_max.cc", "queue_with_max.tsv",
                         &QueueTester, DefaultComparator{}, param_names);
}
