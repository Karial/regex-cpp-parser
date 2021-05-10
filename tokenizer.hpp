#ifndef REGEX_PARSER_TOKENIZER_HPP
#define REGEX_PARSER_TOKENIZER_HPP

#include <istream>
#include <memory>
#include <variant>

class SymbolToken {
 public:
  SymbolToken() = default;
  explicit SymbolToken(char sym) : sym(sym) {}
  bool operator==(const SymbolToken &rhs) const { return sym == rhs.sym; }

 public:
  char sym{};
};

class PlusToken {
 public:
  bool operator==(const PlusToken &rhs) const { return true; }
};

class StarToken {
 public:
  bool operator==(const StarToken &rhs) const { return true; }
};

class OrToken {
 public:
  bool operator==(const OrToken &rhs) const { return true; }
};

class SpecialSymbolToken {
 public:
  bool operator==(const SpecialSymbolToken &) const { return true; }
};

class Empty {
 public:
  bool operator==(const Empty &) const { return true; }
};

enum class BracketToken { OPEN, CLOSE };

typedef std::variant<SymbolToken, PlusToken, StarToken, OrToken, BracketToken,
                     Empty>
    Token;

class Tokenizer {
 public:
  explicit Tokenizer(std::istream *in) : stream_(in) { Next(); }

  void Next() {
    if ((stream_->peek() == EOF)) {
      currentToken_ = Empty{};
      return;
    }
    switch (stream_->peek()) {
      case '+': {
        stream_->get();
        currentToken_ = PlusToken{};
        break;
      }
      case '*': {
        stream_->get();
        currentToken_ = StarToken{};
        break;
      }
      case '|': {
        stream_->get();
        currentToken_ = OrToken{};
        break;
      }
      case '(': {
        stream_->get();
        currentToken_ = BracketToken::OPEN;
        break;
      }
      case ')': {
        stream_->get();
        currentToken_ = BracketToken::CLOSE;
        break;
      }
      case '\\': {
      }
      default:currentToken_ = SymbolToken{static_cast<char>(stream_->get())};
        break;
    }
  }

  Token GetToken() const { return currentToken_; }

 private:
  std::istream *stream_;
  Token currentToken_;
};

#endif  // REGEX_PARSER_TOKENIZER_HPP
