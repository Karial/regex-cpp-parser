#include <iostream>
#include <sstream>

#include "ast.hpp"
#include "tokenizer.hpp"

int main() {
  //  auto in = std::make_unique<std::stringstream>("(a+b*c|d)");
  //  Tokenizer tokenizer(std::move(in));
  //  Token token = tokenizer.GetToken();
  //  while (token.index() != 5) {
  //    switch (token.index()) {
  //      case 0:
  //        std::cerr << "Symbol\n";
  //        break;
  //      case 1:
  //        std::cerr << "Plus\n";
  //        break;
  //      case 2:
  //        std::cerr << "Star\n";
  //        break;
  //      case 3:
  //        std::cerr << "Or\n";
  //        break;
  //      case 4:
  //        std::cerr << "Bracket\n";
  //        break;
  //      case 5:
  //        std::cerr << "Empty\n";
  //        break;
  //    }
  //    tokenizer.Next();
  //    token = tokenizer.GetToken();
  //  }
  std::stringstream in("abc((d|e*)|(f+|g))l*");
  auto result = CreateASTFromStream(&in);
  std::cout << result->Output() << '\n';
  return 0;
}
