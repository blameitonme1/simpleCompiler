#ifndef SYNTAX_PARSER_H
#define SYNTAX_PARSER_H

#include <vector>
#include <unordered_map>
#include <string>
#include "lexical_parser.h"

// 全局变量声明
extern size_t currentToken;

// 函数声明
bool match(tokenType expectedType);
void error(const std::string& message);

int program();
int declarationList();
int declaration();
int varDeclaration();
int typeSpecifier();
int funDeclaration();
int params();
int paramList();
int param();
int compoundStmt();
int localDeclarations();
int statementList();
int statement();
int expressionStmt();
int selectionStmt();
int iterationStmt();
int returnStmt();
int expression();
int var();
int simpleExpression();
int additiveExpression();
int term();
int factor();
int call();
int relop();
int args();
int argList();

#endif
