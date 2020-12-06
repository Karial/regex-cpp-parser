#include <iostream>
#include <sstream>

#include "ast.hpp"
#include "tokenizer.hpp"

int main() {
  std::stringstream in("abc((d|e*)|(f+|g))l*");
  auto result = CreateASTFromStream(&in);
  std::cout << result->Output() << '\n';
  return 0;
}
