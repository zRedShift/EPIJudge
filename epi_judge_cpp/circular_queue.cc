#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
class Queue {
 public:
  explicit Queue(size_t capacity) : queue(capacity), front(), size() {}
  void Enqueue(int x) {
    if (size == queue.size()) {
      auto iter = queue.begin() + front % queue.size();
      std::vector<int> new_queue;
      new_queue.reserve(2 * size);
      new_queue.insert(new_queue.end(), iter, queue.end());
      new_queue.insert(new_queue.end(), queue.begin(), iter);
      new_queue.resize(2 * size);
      queue.swap(new_queue);
      front = 0;
    }
    queue[(front + size++) % queue.size()] = x;
  }
  int Dequeue() {
    --size;
    return queue[front++ % queue.size()];
  }
  [[nodiscard]] int Size() const { return size; }
 private:
  std::vector<int> queue;
  size_t front, size;
};
struct QueueOp {
  enum { kConstruct, kDequeue, kEnqueue, kSize } op;
  int argument;

  QueueOp(const std::string &op_string, int arg) : argument(arg) {
    if (op_string == "Queue") {
      op = kConstruct;
    } else if (op_string == "dequeue") {
      op = kDequeue;
    } else if (op_string == "enqueue") {
      op = kEnqueue;
    } else if (op_string == "size") {
      op = kSize;
    } else {
      throw std::runtime_error("Unsupported queue operation: " + op_string);
    }
  }

  void execute(Queue &q) const {
    switch (op) {
      case kConstruct:
        // Hack to bypass deleted assign operator
        q.~Queue();
        new(&q) Queue(argument);
        break;
      case kDequeue: {
        int result = q.Dequeue();
        if (result != argument) {
          throw TestFailure("Dequeue: expected " + std::to_string(argument) +
              ", got " + std::to_string(result));
        }
      }
        break;
      case kEnqueue:q.Enqueue(argument);
        break;
      case kSize: {
        int s = q.Size();
        if (s != argument) {
          throw TestFailure("Size: expected " + std::to_string(argument) +
              ", got " + std::to_string(s));
        }
      }
        break;
    }
  }
};

template<>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp> &ops) {
  Queue q(0);
  for (auto &op : ops) {
    op.execute(q);
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "circular_queue.cc", "circular_queue.tsv",
                         &QueueTester, DefaultComparator{}, param_names);
}
