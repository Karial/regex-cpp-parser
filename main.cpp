#include <iostream>
#include <sstream>

#include "finite_automata.hpp"
#include "character_classes.hpp"

void Check(const FiniteAutomata &automata, const std::string &text) {
  if (automata.Check(text)) {
    std::cout << "Text " << text << " match regex\n";
  } else {
    std::cout << "Text " << text << " does not match regex\n";
  }
}

int main() {
  std::stringstream dfa_in("a*b+");
  auto dfa = CreateDFAFromStream(&dfa_in);
  std::stringstream nfa_in("a*b+");
  auto nfa = CreateNFAFromStream(&nfa_in);
  Check(dfa, "aaab");
  Check(nfa, "aa");
  return 0;
}
