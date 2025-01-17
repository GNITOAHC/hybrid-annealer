#pragma once

#include <map>
#include <vector>

namespace sa {

enum Spin { UP = 1, DOWN = -1 };
typedef struct AdjNode {
    int val;
    double weight;
    AdjNode *next;
    AdjNode (int v, double w) : val(v), weight(w), next(nullptr) {}
} AdjNode;

class Graph {
  public:
    std::vector<AdjNode *> adjList; // adjList[i] is the head of the adjacency list of vertex i
    std::map<int, std::vector<int> > adjMap;
    std::map<int, double> constant_map; // index of node -> constant
    std::vector<Spin> spins;            // vector of spins (sorted by index)
    double constant;
    int length; // Length of the graph

    double getHamiltonianEnergy() const;
    double getHamiltonianDifference(const int&) const; // Get Ham diff if spin is flipped

    void pushBack(const double&);                         // Push back a constant
    void pushBack(const int&, const int&, const double&); // Push back an edge
    void pushBack(const int&, const double&);             // Push back a constant_map

    void flip(const int&); // Flip the spin of a node
};

} // namespace sa
