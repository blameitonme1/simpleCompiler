#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "lexical_parser.h"


std::vector<Token> tokens;
size_t currentToken = 0; // 下标指向当前的token

bool match(tokenType expectedType) {
    // 匹配非终结符
    if (currentToken < tokens.size() && tokens[currentToken].type == expectedType) {
        currentToken++;
        return true;
    }
    return false;
}

void error(const std::string& message) {
    // 报错函数
    std::cerr << "Error at token " << currentToken << " " << tokens[currentToken].lexeme << ": " << message << std::endl;
    exit(1);
}

void program();
void declarationList();
int declaration();
void varDeclaration();
void typeSpecifier();
void funDeclaration();
void params();
void paramList();
void param();
void compoundStmt();
void localDeclarations();
void statementList();
void statement();
void expressionStmt();
void selectionStmt();
void iterationStmt();
void returnStmt();
void expression();
void var();
void simpleExpression();
void additiveExpression();
void term();
void factor();
void call();
void args();
void argList();

// Sample function implementations
void program() {
    declarationList();
    if (currentToken != tokens.size()) {
        error("Extra code after program end.");
    }
    else{
        std::cout << "no errors are detected." << std::endl; // 成功匹配
    }
}

void declarationList() {
    do {
        declaration();
    } while (currentToken < tokens.size());
}

int declaration() {
    if (match(KEYWORD)) {
        typeSpecifier(); //识别类型
        if (match(IDENTIFIER)) {
            // Move token consumption logic into the specific function branches
            Token &token = tokens[currentToken - 1]; // Access the previously matched identifier
            if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
                // Function declaration
                currentToken--; // Unconsume ID
                funDeclaration();
            } else {
                // Variable declaration
                currentToken--; // Unconsume ID
                varDeclaration();
            }
        } else {
            error("Expected identifier after type specifier."); // 应给出标识符
        }
    } else {
        error("Expected type specifier in declaration."); // 声明应该报类型
    }
}

void varDeclaration() {
    typeSpecifier(); // 解析变量类型
    if (!match(IDENTIFIER)) { // 匹配变量名
        error("Expected identifier after type specifier.");
    }

    if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "[") { // 可能是数组声明
        if (!match(NUMBER)) { // 数组大小
            error("Expected array size after '['.");
        }
        if (!match(DELIMITER) || tokens[currentToken - 1].lexeme != "]") { // 数组声明结束
            error("Expected ']' after array size.");
        }
    }

    if (!match(DELIMITER) || tokens[currentToken - 1].lexeme != ";") { // 匹配分号
        error("Expected ';' after variable declaration.");
    }
}



void funDeclaration() {
    // 匹配函数声明
    typeSpecifier();
    if (match(IDENTIFIER)) {
        if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
            // Function declaration with parameters
            params();
        } else {
            error("Expected '(' after function name.");
        }
    } else if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "{") {
        // Function declaration with compound statement
        compoundStmt();
    } else {
        error("Expected identifier or compound statement in function declaration.");
    }
}

