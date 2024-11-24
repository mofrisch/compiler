//
// Created by mo on 22.11.24.
//
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <unordered_set>

// Token types enum
enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SYMBOL,
    STRING_LITERAL,
    UNKNOWN
};

// Struct representing a Token
struct Token {
    TokenType type;
    std::string value;
    size_t line;
};

// Set of C keywords
const std::unordered_set<std::string> keywords = {
    "int", "return", "if", "else", "for", "while", "do", "break", "continue", "void", "char", "float", "double"
};

// Set of operators
const std::unordered_set<char> operators = {
    '+', '-', '*', '/', '=', '!', '<', '>'
};

// Set of symbols
const std::unordered_set<char> symbols = {
    '(', ')', '{', '}', '[', ']', ';', ','
};


// Lexer function to tokenize the input string
std::vector<Token> lexer(const std::string& input) {
    std::vector<Token> tokens;
    size_t i = 0;
size_t line = 1;

    while (i < input.length()) {
        char current = input[i];

        // count lines
        if (current == '\n') {
            line++;
            i++;
            continue;
        }

        // Skip whitespaces
        if (isspace(current)) {
            i++;
            continue;
        }

        // Keywords and Identifiers
        if (isalpha(current) || current == '_') {
            std::string identifier;
            while (i < input.length() && (isalnum(input[i]) || input[i] == '_')) {
                identifier += input[i++];
            }
            if (keywords.contains(identifier)) {
                tokens.push_back({TokenType::KEYWORD, identifier, line});
            } else {
                tokens.push_back({TokenType::IDENTIFIER, identifier,line});
            }
            continue;
        }

        // Numbers
        if (isdigit(current)) {
            std::string number;
            while (i < input.length() && isdigit(input[i])) {
                number += input[i++];
            }
            tokens.push_back({TokenType::NUMBER, number,line});
            continue;
        }

        // Operators
        if (operators.contains(current)) {
            tokens.push_back({TokenType::OPERATOR, std::string(1, current),line});
            i++;
            continue;
        }

        // Symbols
        if (symbols.contains(current)) {
            tokens.push_back({TokenType::SYMBOL, std::string(1, current),line});
            i++;
            continue;
        }

        // String Literals
        if (current == '"') {
            std::string str_literal;
            str_literal += current; // add the opening quote
            i++;
            while (i < input.length() && input[i] != '"') {
                str_literal += input[i++];
            }
            if (i < input.length() && input[i] == '"') {
                str_literal += input[i++]; // add the closing quote
            }
            tokens.push_back({TokenType::STRING_LITERAL, str_literal,line});
            continue;
        }

        // Unknown tokens
        tokens.push_back({TokenType::UNKNOWN, std::string(1, current),line});
        i++;
    }
    return tokens;
}

// Function to print tokens
void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Line: " <<  token.line <<  " Token Type: ";
        switch (token.type) {
            case TokenType::KEYWORD: std::cout << "KEYWORD"; break;
            case TokenType::IDENTIFIER: std::cout << "IDENTIFIER"; break;
            case TokenType::NUMBER: std::cout << "NUMBER"; break;
            case TokenType::OPERATOR: std::cout << "OPERATOR"; break;
            case TokenType::SYMBOL: std::cout << "SYMBOL"; break;
            case TokenType::STRING_LITERAL: std::cout << "STRING_LITERAL"; break;
            case TokenType::UNKNOWN: std::cout << "UNKNOWN"; break;
        }
        std::cout << ", Value: " << token.value << ")" << std::endl;
    }
}



// Main function
int main() {
    std::string code = "int main() { int x = 42;\n(x > 0) return x; }";
    std::vector<Token> tokens = lexer(code);
    printTokens(tokens);
    return 0;
}
