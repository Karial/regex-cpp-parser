#include <iostream>
#include <sstream>

#include "finite_automata.hpp"

int main() {
  std::stringstream in("a*b+");
  auto result = CreateASTFromStream(&in);
  auto fa = CreateNFAFromAST(result.get());
  if (fa.Check("ab")) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  if (fa.Check("abc")) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  if (fa.Check("a")) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  if (fa.Check("b")) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  if (fa.Check("aaaaabbbbbbbbbbbbb")) {
    std::cout << "Yes" << '\n';
  } else {
    std::cout << "No" << '\n';
  }
  //  std::stringstream in("(a*b+)|c");
  //  auto result = CreateASTFromStream(&in);
  //  auto fa = CreateNFAFromAST(result.get());
  //  if (fa.Check("ab")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("c")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("a")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("b")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("aaaaabbbbbbbbbbbbb")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  std::stringstream in("(a+|b*)");
  //  auto result = CreateASTFromStream(&in);
  //  auto fa = CreateNFAFromAST(result.get());
  //  if (fa.Check("")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("c")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("a")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("b")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  //  if (fa.Check("aaaaabbbbbbbbbbbbb")) {
  //    std::cout << "Yes" << '\n';
  //  } else {
  //    std::cout << "No" << '\n';
  //  }
  return 0;
}
