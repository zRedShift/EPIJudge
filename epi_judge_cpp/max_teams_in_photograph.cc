#include <stdexcept>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"
using std::vector;

struct GraphVertex {
  vector<GraphVertex *> edges;
  // Set max_distance = 0 to indicate unvisited vertex.
  int max_distance = 0;
};
int Accumulator(const int &, GraphVertex *);

int MaxLength(GraphVertex *v) {
  return v->max_distance = 1 + std::accumulate(v->edges.begin(), v->edges.end(), 0, Accumulator);
}

int Accumulator(const int &max, GraphVertex *v) {
  return std::max(max, v->max_distance ? v->max_distance : MaxLength(v));
}

int FindLargestNumberTeams(vector<GraphVertex> *graph) {
  return std::accumulate(graph->begin(), graph->end(), 0, [](const auto &m, auto &v) { return Accumulator(m, &v); });
}
struct Edge {
  int from;
  int to;
};

template<>
struct SerializationTraits<Edge> : UserSerTraits<Edge, int, int> {};

int FindLargestNumberTeamsWrapper(TimedExecutor &executor, int k,
                                  const vector<Edge> &edges) {
  if (k <= 0) {
    throw std::runtime_error("Invalid k value");
  }

  vector<GraphVertex> graph(k, GraphVertex{});

  for (const Edge &e : edges) {
    if (e.from < 0 || e.from >= k || e.to < 0 || e.to >= k) {
      throw std::runtime_error("Invalid vertex index");
    }
    graph[e.from].edges.push_back(&graph[e.to]);
  }

  return executor.Run([&] { return FindLargestNumberTeams(&graph); });
}

int main(int argc, char *argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "k", "edges"};
  return GenericTestMain(
      args, "max_teams_in_photograph.cc", "max_teams_in_photograph.tsv",
      &FindLargestNumberTeamsWrapper, DefaultComparator{}, param_names);
}
