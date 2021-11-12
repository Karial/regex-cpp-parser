# regex-cpp-parser
Simple regex cpp parser library. 
Usage:

* Using deterministic automata 
```c++
std::stringstream in("a*b+");
auto fastDfa = CreateFastFiniteAutomataFromStream(&in);
fastDfa.Check("aaabbbb"); // true
fastDfa.Check("aaba");    // false
fastDfa.Check("abc");     // false
```
  
* Using non deterministic automata
```c++
std::stringstream in("a*b+");
auto ast = CreateASTFromStream(&in);
auto nfa = CreateNFAFromAST(ast.get());
nfa.Check("aaabbbb"); // true
nfa.Check("aaba");    // false
nfa.Check("abc");     // false
```
