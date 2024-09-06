%{
#include <iostream>
#include <string>
#include <vector>

// Define token types
enum TokenType {
    INT,
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    SEMICOLON,
    ASSIGN,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;

    // Default constructor
    Token() : type(TokenType::END_OF_FILE), value("") {}

    // Constructor with arguments
    Token(TokenType type, const std::string& value = "")
        : type(type), value(value) {}
};

// To store a list of tokens
std::vector<Token> token_list;

%}

%%
"int"            { token_list.push_back(Token(INT, "int")); }
"+"              { token_list.push_back(Token(PLUS, "+")); }
"-"              { token_list.push_back(Token(MINUS, "-")); }
"*"              { token_list.push_back(Token(STAR, "*")); }
"/"              { token_list.push_back(Token(SLASH, "/")); }
";"              { token_list.push_back(Token(SEMICOLON, ";")); }
"="              { token_list.push_back(Token(ASSIGN, "=")); }

[0-9]+           { token_list.push_back(Token(NUMBER, yytext)); }
[a-zA-Z_][a-zA-Z0-9_]*  { token_list.push_back(Token(IDENTIFIER, yytext)); }

[ \t\n]+         ;  // Skip whitespace
.                { std::cerr << "Unrecognized character: " << yytext << std::endl; exit(1); }

%%

int main(int argc, char **argv) {
  FILE *fd = stdin;
  if (argc > 1 && (fd = fopen(argv[1], "r")) == NULL)
    exit(EXIT_FAILURE);
  // create a lexer that consumes a file or reads stdin
  yyFlexLexer lexer(fd);
  // here we go!
  lexer.yylex();
  // display the results
  //lexer.results();
  if (fd != stdin)
    fclose(fd);
  return 0;
}



