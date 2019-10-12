#include <stdexcept>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
using std::vector;

struct GraphVertex {
  uint8_t d = 2;
  vector<GraphVertex *> edges;
};

bool ColorVertex(GraphVertex &vertex, bool color) {
  if (vertex.d != 2)
    return vertex.d == color;
  vertex.d = color;
  color = !color;
  return std::all_of(vertex.edges.begin(), vertex.edges.end(), [color](auto &v) { return ColorVertex(*v, color); });
}

bool IsAnyPlacementFeasible(vector<GraphVertex> *graph) {
  return std::all_of(graph->begin(), graph->end(), [](auto &v) { return v.d != 2 || ColorVertex(v, false); });
}
struct Edge {
  int from;
  int to;
};

template<>
struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

bool IsAnyPlacementFeasibleWrapper(TimedExecutor &executor, int k,
                                   const vector<Edge> &edges) {
  vector<GraphVertex> graph;
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }
  graph.reserve(k);

  for (int i = 0; i < k; i++) {
    graph.push_back(GraphVertex{});
  }

  for (auto &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return IsAnyPlacementFeasible(&graph); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "edges"};
  return GenericTestMain(
      args, "is_circuit_wirable.cc", "is_circuit_wirable.tsv",
      &IsAnyPlacementFeasibleWrapper, DefaultComparator{}, param_names);
}
