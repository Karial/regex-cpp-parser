#ifndef REGEX_PARSER_FINITE_AUTOMATA_HPP
#define REGEX_PARSER_FINITE_AUTOMATA_HPP
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ast.hpp"
#include "util.hpp"

class FiniteAutomata {
public:
  bool Check(const std::string &str) {
    std::vector<Node *> currentNodes = {begin};

    for (const auto &c : str) {
      currentNodes = Node::GetSurrounding(currentNodes);
      currentNodes = Node::Move(currentNodes, c);
    }

    currentNodes = Node::GetSurrounding(currentNodes);
    for (const auto &node : currentNodes) {
      if (node->isFinal) {
        return true;
      }
    }

    return false;
  }

  friend FiniteAutomata CreateNFAFromAST(ASTNode *ast);
  friend FiniteAutomata CreateDFAFromNFA(const FiniteAutomata &nfa);

private:
  struct Node {
    bool isFinal{false};
    std::unordered_map<char, std::vector<Node *>> next;

    static std::vector<Node *> GetSurrounding(std::vector<Node *> nodes) {
      std::unordered_set<Node *> used;
      for (size_t i = 0; i < nodes.size(); ++i) {
        if (used.find(nodes[i]) == std::end(used)) {
          used.insert(nodes[i]);
          if (nodes[i]->next.find(0) != std::end(nodes[i]->next)) {
            nodes.insert(std::end(nodes), std::begin(nodes[i]->next[0]),
                         std::end(nodes[i]->next[0]));
          }
        }
      }

      std::unordered_set<Node *> unique(std::begin(nodes), std::end(nodes));
      nodes.assign(std::begin(unique), std::end(unique));

      return nodes;
    }

    static std::vector<Node *> Move(const std::vector<Node *> &nodes,
                                    char sym) {
      std::vector<Node *> newNodes;
      for (const auto &node : nodes) {
        if (node->next.find(sym) != std::end(node->next)) {
          newNodes.insert(std::end(newNodes), std::begin(node->next[sym]),
                          std::end(node->next[sym]));
        }
      }
      std::unordered_set<Node *> unique(std::begin(newNodes),
                                        std::end(newNodes));
      newNodes.assign(std::begin(unique), std::end(unique));

      return newNodes;
    }
  };

  Node *begin{};
  Node *end{};
};

FiniteAutomata CreateNFAFromAST(ASTNode *ast) {
  FiniteAutomata result;
  if (auto b = dynamic_cast<SymbolNode *>(ast)) {
    result.begin = new FiniteAutomata::Node;
    result.end = new FiniteAutomata::Node{true};
    result.begin->next[b->GetSym()] = {result.end};
  }
  if (auto b = dynamic_cast<OrNode *>(ast)) {
    result.begin = new FiniteAutomata::Node;
    result.end = new FiniteAutomata::Node{true};
    auto leftAutomata = CreateNFAFromAST(b->GetLhs().get());
    auto rightAutomata = CreateNFAFromAST(b->GetRhs().get());
    result.begin->next[0] = {leftAutomata.begin, rightAutomata.begin};
    leftAutomata.end->next[0] = {result.end};
    rightAutomata.end->next[0] = {result.end};
    leftAutomata.end->isFinal = rightAutomata.end->isFinal = false;
  }
  if (auto b = dynamic_cast<ConcatNode *>(ast)) {
    auto leftAutomata = CreateNFAFromAST(b->GetLhs().get());
    auto rightAutomata = CreateNFAFromAST(b->GetRhs().get());
    result.begin = leftAutomata.begin;
    result.end = rightAutomata.end;
    leftAutomata.end->next[0].emplace_back(rightAutomata.begin);
    leftAutomata.end->isFinal = false;
  }
  if (auto b = dynamic_cast<PlusNode *>(ast)) {
    auto valueAutomata = CreateNFAFromAST(b->GetValue().get());
    result.begin = new FiniteAutomata::Node;
    result.end = new FiniteAutomata::Node{true};
    result.begin->next[0] = {valueAutomata.begin};
    valueAutomata.end->next[0].emplace_back(result.end);
    valueAutomata.end->next[0].emplace_back(valueAutomata.begin);
    valueAutomata.end->isFinal = false;
  }
  if (auto b = dynamic_cast<StarNode *>(ast)) {
    auto valueAutomata = CreateNFAFromAST(b->GetValue().get());
    result.begin = new FiniteAutomata::Node;
    result.end = new FiniteAutomata::Node{true};
    result.begin->next[0] = {valueAutomata.begin};
    valueAutomata.end->next[0].emplace_back(result.end);
    valueAutomata.end->next[0].emplace_back(valueAutomata.begin);
    result.begin->next[0].emplace_back(result.end);
    valueAutomata.end->isFinal = false;
  }
  return result;
}

FiniteAutomata CreateDFAFromNFA(const FiniteAutomata &nfa) {
  typedef std::vector<FiniteAutomata::Node *> State;

  FiniteAutomata result;
  std::unordered_map<State, FiniteAutomata::Node *, SetHash<State>> newNodes;
  std::unordered_set<State, SetHash<State>> usedStates;
  std::queue<State> sequence;

  auto startState = FiniteAutomata::Node::GetSurrounding({nfa.begin});
  sequence.push(startState);
  result.begin = newNodes[startState] = new FiniteAutomata::Node;

  while (!sequence.empty()) {
    auto currentState = sequence.front();
    usedStates.insert(currentState);
    sequence.pop();

    std::unordered_set<char> usedSymbols;
    for (const auto &node : currentState) {
      for (const auto &[sym, _] : node->next) {
        if (sym != 0 && usedSymbols.find(sym) == end(usedSymbols)) {
          usedSymbols.insert(sym);
        }
      }
    }

    for (const auto &sym : usedSymbols) {
      auto newState = FiniteAutomata::Node::GetSurrounding(
          FiniteAutomata::Node::Move(currentState, sym));
      if (!newState.empty()) {
        if (newNodes.find(newState) == end(newNodes)) {
          sequence.push(newState);
          newNodes[newState] = new FiniteAutomata::Node;
        }
      }

      newNodes[currentState]->next[sym] = {newNodes[newState]};
    }
  }

  for (const auto &[state, node] : newNodes) {
    if (std::find(begin(state), end(state), nfa.end) != end(state)) {
      node->isFinal = true;
    }
  }

  return result;
}

#endif