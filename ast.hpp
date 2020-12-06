#ifndef REGEX_PARSER_AST_HPP
#define REGEX_PARSER_AST_HPP

#include <memory>
#include <vector>

#include "tokenizer.hpp"

class Node {
 public:
  virtual std::string Output() const = 0;
};

class SymbolNode : public Node {
 public:
  explicit SymbolNode(char sym) : sym(sym) {}
  std::string Output() const override {
    std::string result = "Symbol node(";
    result += sym;
    result += ")";
    return result;
  }

 private:
  char sym;
};

class OrNode : public Node {
 public:
  OrNode(std::unique_ptr<Node>&& lhs, std::unique_ptr<Node>&& rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
  std::string Output() const override {
    std::string result =
        "OrNode(" + lhs_->Output() + ", " + rhs_->Output() + ")";
    return result;
  }

 private:
  std::unique_ptr<Node> lhs_{}, rhs_{};
};

class ConcatNode : public Node {
 public:
  ConcatNode(std::unique_ptr<Node>&& lhs, std::unique_ptr<Node>&& rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
  std::string Output() const override {
    std::string result =
        "ConcatNode(" + lhs_->Output() + ", " + rhs_->Output() + ")";
    return result;
  }

 private:
  std::unique_ptr<Node> lhs_, rhs_;
};

class PlusNode : public Node {
 public:
  explicit PlusNode(std::unique_ptr<Node>&& value) : value_(std::move(value)) {}
  std::string Output() const override {
    std::string result = "PlusNode(" + value_->Output() + ")";
    return result;
  }

 private:
  std::unique_ptr<Node> value_;
};

class StarNode : public Node {
 public:
  explicit StarNode(std::unique_ptr<Node>&& value) : value_(std::move(value)) {}
  std::string Output() const override {
    std::string result = "StarNode(" + value_->Output() + ")";
    return result;
  }

 private:
  std::unique_ptr<Node> value_;
};

std::unique_ptr<Node> CreateASTFromTokenizer(Tokenizer& tokenizer) {
  Token token = tokenizer.GetToken();
  std::vector<std::vector<std::unique_ptr<Node>>> tokensSequences(1);
  bool finish = false;
  while (token.index() != 5 && !finish) {
    switch (token.index()) {
      case 0: {
        tokensSequences.back().emplace_back(
            std::make_unique<SymbolNode>(std::get<SymbolToken>(token).sym));
        break;
      }
      case 1: {
        auto lastToken = std::move(tokensSequences.back().back());
        tokensSequences.back().back() =
            std::make_unique<PlusNode>(std::move(lastToken));
        break;
      }
      case 2: {
        auto lastToken = std::move(tokensSequences.back().back());
        tokensSequences.back().back() =
            std::make_unique<StarNode>(std::move(lastToken));
        break;
      }
      case 3: {
        tokensSequences.emplace_back();
        break;
      }
      case 4: {
        if (std::get<BracketToken>(token) == BracketToken::OPEN) {
          tokenizer.Next();
          tokensSequences.back().emplace_back(
              CreateASTFromTokenizer(tokenizer));
          break;
        } else {
          finish = true;
          break;
        }
      }
      case 5: {
        finish = true;
        break;
      }
    }
    tokenizer.Next();
    token = tokenizer.GetToken();
  }
  std::vector<std::unique_ptr<Node>> tokens;
  for (auto& tokensSequence : tokensSequences) {
    auto resultToken = std::move(tokensSequence[0]);
    for (size_t i = 1; i < tokensSequence.size(); ++i) {
      resultToken = std::make_unique<ConcatNode>(std::move(resultToken),
                                                 std::move(tokensSequence[i]));
    }
    tokens.emplace_back(std::move(resultToken));
  }
  auto result = std::move(tokens[0]);
  for (size_t i = 1; i < tokens.size(); ++i) {
    result = std::make_unique<OrNode>(std::move(result), std::move(tokens[i]));
  }

  return result;
}

std::unique_ptr<Node> CreateASTFromStream(std::istream* in) {
  Tokenizer tokenizer(in);
  std::unique_ptr<Node> result = CreateASTFromTokenizer(tokenizer);
  return result;
}

#endif  // REGEX_PARSER_AST_HPP
