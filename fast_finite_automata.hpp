//
// Created by Aleksandr Kazakov on 5/18/21.
//

#ifndef REGEX_PARSER__FAST_FINITE_AUTOMATA_HPP_
#define REGEX_PARSER__FAST_FINITE_AUTOMATA_HPP_

#include "finite_automata.hpp"

#include <vector>
#include <unordered_map>
#include <unordered_set>

class FastFiniteAutomata {
 public:
  bool Check(const std::string &str) const {
    size_t currentNode = begin;

    for (const auto &c : str) {
      auto &nextNodes = allNodes[currentNode].next;

#ifndef SAVE_MEMORY
      if (nextNodes[static_cast<unsigned char>(c)] == SIZE_T_MAX) {
        return false;
      }

      currentNode = nextNodes[static_cast<unsigned char>(c)];
#else
      if (nextNodes.find(static_cast<unsigned char>(c)) == std::end(nextNodes)) {
        return false;
      }

      currentNode = nextNodes.at(static_cast<unsigned char>(c));
#endif
    }

    return allNodes[currentNode].isFinal;
  }

  struct Node {
    bool isFinal{false};
#ifndef SAVE_MEMORY
    std::vector<size_t> next = std::vector<size_t>(256, SIZE_T_MAX);
#else
    std::unordered_map<char, size_t> next;
#endif
  };

  friend FastFiniteAutomata CreateFastFiniteAutomata(const FiniteAutomata &automata);
 private:
  size_t begin{0}, end{SIZE_T_MAX};
  std::vector<Node> allNodes;
  bool isDeterministic{false};
};

FastFiniteAutomata CreateFastFiniteAutomata(const FiniteAutomata &dfa) {
  assert(dfa.isDeterministic);

  std::unordered_map<FiniteAutomata::Node *, size_t> nodeNum;
  FastFiniteAutomata res;
  res.allNodes.emplace_back(FastFiniteAutomata::Node{});

  using Node = FiniteAutomata::Node;
  std::queue<std::pair<Node *, size_t>> nodesQueue;
  std::unordered_map<Node *, size_t> nodesMap;
  nodesQueue.emplace(dfa.begin, res.begin);
  nodesMap[dfa.begin] = res.begin;
  res.allNodes[0].isFinal = dfa.begin->isFinal;
  while (!nodesQueue.empty()) {
    auto[node, copyNode] = nodesQueue.front();
    nodesQueue.pop();
    for (const auto &[k, nextNodes]: node->next) {
      for (const auto &nextNode : nextNodes) {
        if (!nodesMap[nextNode]) {
          nodesMap[nextNode] = res.allNodes.size();
          res.allNodes.emplace_back(FastFiniteAutomata::Node{});
          res.allNodes[nodesMap[nextNode]].isFinal = nextNode->isFinal;
          nodesQueue.emplace(nextNode, nodesMap[nextNode]);
        }
        res.allNodes[copyNode].next[static_cast<unsigned char>(k)] = nodesMap[nextNode];
      }
    }
  }
  res.end = nodesMap[dfa.end];

  return res;
}

FastFiniteAutomata CreateFastFiniteAutomataFromStream(std::istream *in) {
  return CreateFastFiniteAutomata(CreateDFAFromStream(in));
}

#endif //REGEX_PARSER__FAST_FINITE_AUTOMATA_HPP_
