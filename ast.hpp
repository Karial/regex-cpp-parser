#ifndef REGEX_PARSER_AST_HPP
#define REGEX_PARSER_AST_HPP

#include <memory>
#include <vector>

#include "tokenizer.hpp"

class ASTNode {
public:
  virtual std::string Output() const = 0;
};

class SymbolNode : public ASTNode {
public:
  explicit SymbolNode(char sym) : sym(sym) {}

  std::string Output() const override {
    std::string result = "Symbol node(";
    result += sym;
    result += ")";
    return result;
  }

  char GetSym() const { return sym; }

private:
  char sym;
};

class OrNode : public ASTNode {
public:
  OrNode(std::unique_ptr<ASTNode> &&lhs, std::unique_ptr<ASTNode> &&rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
  std::string Output() const override {
    std::string result =
        "OrNode(" + lhs_->Output() + ", " + rhs_->Output() + ")";
    return result;
  }

  const std::unique_ptr<ASTNode> &GetLhs() const { return lhs_; }
  const std::unique_ptr<ASTNode> &GetRhs() const { return rhs_; }

private:
  std::unique_ptr<ASTNode> lhs_{}, rhs_{};
};

class ConcatNode : public ASTNode {
public:
  ConcatNode(std::unique_ptr<ASTNode> &&lhs, std::unique_ptr<ASTNode> &&rhs)
      : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}
  std::string Output() const override {
    std::string result =
        "ConcatNode(" + lhs_->Output() + ", " + rhs_->Output() + ")";
    return result;
  }

  const std::unique_ptr<ASTNode> &GetLhs() const { return lhs_; }
  const std::unique_ptr<ASTNode> &GetRhs() const { return rhs_; }

private:
  std::unique_ptr<ASTNode> lhs_, rhs_;
};

class PlusNode : public ASTNode {
public:
  explicit PlusNode(std::unique_ptr<ASTNode> &&value)
      : value_(std::move(value)) {}
  std::string Output() const override {
    std::string result = "PlusNode(" + value_->Output() + ")";
    return result;
  }

  const std::unique_ptr<ASTNode> &GetValue() const { return value_; }

private:
  std::unique_ptr<ASTNode> value_;
};

class StarNode : public ASTNode {
public:
  explicit StarNode(std::unique_ptr<ASTNode> &&value)
      : value_(std::move(value)) {}
  std::string Output() const override {
    std::string result = "StarNode(" + value_->Output() + ")";
    return result;
  }

  const std::unique_ptr<ASTNode> &GetValue() const { return value_; }

private:
  std::unique_ptr<ASTNode> value_;
};

std::unique_ptr<ASTNode> CreateASTFromTokenizer(Tokenizer &tokenizer) {
  Token token = tokenizer.GetToken();
  std::vector<std::vector<std::unique_ptr<ASTNode>>> tokensSequences(1);
  while (token.index() != 5) {
    if (std::holds_alternative<SymbolToken>(token)) {
      tokensSequences.back().emplace_back(
          std::make_unique<SymbolNode>(std::get<SymbolToken>(token).sym));
    } else if (std::holds_alternative<PlusToken>(token)) {
      auto lastToken = std::move(tokensSequences.back().back());
      tokensSequences.back().back() =
          std::make_unique<PlusNode>(std::move(lastToken));
    } else if (std::holds_alternative<StarToken>(token)) {
      auto lastToken = std::move(tokensSequences.back().back());
      tokensSequences.back().back() =
          std::make_unique<StarNode>(std::move(lastToken));
    } else if (std::holds_alternative<OrToken>(token)) {
      tokensSequences.emplace_back();
    } else if (std::holds_alternative<BracketToken>(token)) {
      if (std::get<BracketToken>(token) == BracketToken::OPEN) {
        tokenizer.Next();
        tokensSequences.back().emplace_back(CreateASTFromTokenizer(tokenizer));
      } else {
        break;
      }
    } else {
      break;
    }
    tokenizer.Next();
    token = tokenizer.GetToken();
  }

  std::vector<std::unique_ptr<ASTNode>> tokens;
  for (auto &tokensSequence : tokensSequences) {
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

std::unique_ptr<ASTNode> CreateASTFromStream(std::istream *in) {
  Tokenizer tokenizer(in);
  std::unique_ptr<ASTNode> result = CreateASTFromTokenizer(tokenizer);
  return result;
}

#endif // REGEX_PARSER_AST_HPP
