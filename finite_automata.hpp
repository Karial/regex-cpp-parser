#ifndef REGEX_PARSER_FINITE_AUTOMATA_HPP
#define REGEX_PARSER_FINITE_AUTOMATA_HPP
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ast.hpp"

class FiniteAutomata {
public:
  bool Check(const std::string &str) {
    std::vector<Node *> currentNodes = {begin};
    auto getSurrounding = [&currentNodes]() {
      std::unordered_set<Node *> used;
      for (size_t i = 0; i < currentNodes.size(); ++i) {
        if (used.find(currentNodes[i]) == std::end(used)) {
          used.insert(currentNodes[i]);
          if (currentNodes[i]->next.find(0) !=
              std::end(currentNodes[i]->next)) {
            currentNodes.insert(std::end(currentNodes),
                                std::begin(currentNodes[i]->next[0]),
                                std::end(currentNodes[i]->next[0]));
          }
        }
      }

      currentNodes.erase(
          std::unique(std::begin(currentNodes), std::end(currentNodes)),
          std::end(currentNodes));
    };
    for (const auto &c : str) {
      getSurrounding();
      std::vector<Node *> newCurrentNodes;
      for (const auto &node : currentNodes) {
        if (node->next.find(c) != std::end(node->next)) {
          newCurrentNodes.insert(std::end(newCurrentNodes),
                                 std::begin(node->next[c]),
                                 std::end(node->next[c]));
        }
      }
      currentNodes = newCurrentNodes;
      currentNodes.erase(
          std::unique(std::begin(currentNodes), std::end(currentNodes)),
          std::end(currentNodes));
    }

    getSurrounding();
    for (const auto &node : currentNodes) {
      if (node->isFinal) {
        return true;
      }
    }

    return false;
  }

  friend FiniteAutomata CreateNFAFromAST(ASTNode *ast);

private:
  struct Node {
    bool isFinal{false};
    std::unordered_map<char, std::vector<Node *>> next;
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

#endif