#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "lexical_parser.h"


std::vector<Token> tokens;
size_t currentToken = 0; // 下标指向当前分析的token

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
}

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
int args();
int argList();

// Sample function implementations
int program() {
    declarationList();
    if (currentToken != tokens.size()) {
        error("Extra code after program end.");
        return 1;
    }
    else{
        std::cout << "no errors are detected." << std::endl; // 成功匹配
        return 0;
    }
    return 0;
}

int declarationList() {
    do {
        declaration();
    } while (currentToken < tokens.size());
    return 0;
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
            return 1;
        }
    } else {
        error("Expected type specifier in declaration."); // 声明应该报类型
        return 1;
    }
    return 0;
}

int varDeclaration() {
    typeSpecifier(); // 解析变量类型
    if (!match(IDENTIFIER)) { // 匹配变量名
        error("Expected identifier after type specifier.");
        return 1;
    }

    if (match(DELIMITER)) { // 数组声明
        if(tokens[currentToken - 1].lexeme == "["){
            if (!match(NUMBER)) { // 数组大小
                error("Expected array size after '['.");
                return 1;
            }
            if (!match(DELIMITER) || tokens[currentToken - 1].lexeme != "]") { // 数组声明结束
                error("Expected ']' after array size.");
                return 1;
            }
        }
        else if(tokens[currentToken - 1].lexeme != ";"){
            error("Expected ; after valur declaration");
            return 1;
        }
    }
    return 0;
}

int typeSpecifier(){
    if(!match(KEYWORD)){
        error("expected type specifier (int or void)");
        return 1;
    }
    if(tokens[currentToken - 1].lexeme != "int" || tokens[currentToken - 1].lexeme != "void"){
        error("invalid type");
        return 1;
    }
    return 0;
}

int funDeclaration() {
    // 匹配函数声明
    typeSpecifier();
    if (match(IDENTIFIER)) {
        if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
            // Function declaration with parameters
            params();
        } else {
            error("Expected '(' after function name."); // 没匹配到左括号
            return 1;
        }
    }
    else {
        error("Expected identifier in function declaration.");
        return 1;
    }
    compoundStmt(); // 匹配跟着的复合语句
    return 0;
}

int params(){
    // 匹配参数列表
    if(tokens[currentToken - 1].lexeme == "void"){
        return ; // 前看符号，匹配成功
    }
    if(!paramList()){
        return 1;
    }
    return 0;
}

int paramList(){
    // 匹配有实际参数的时候的参数列表
    param();
    while(match(DELIMITER) && tokens[currentToken - 1].lexeme == ","){
        param();
    }
    return 0;
}

int param(){
    // 匹配单个参数
    typeSpecifier();
    if(!match(IDENTIFIER)){
        error("expect identifier for parameter");
        return 1;
    }
    if(match(DELIMITER) && tokens[currentToken - 1].lexeme == "["){
        if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "]"){
            // 差一个右括号匹配成功
            error("Expected ] after [");
            return 1;
        }
    }
    return 0;
}

int compoundStmt(){
    // 匹配复合语句
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "{"){
        error("Expected { at the begining of compound statement");
        return 1;
    }
    return 0;
}

int localDeclarations() {
    while (true) {
        if (!varDeclaration()) {
            break; // 如果不能匹配 var-declaration，退出循环
        }
    }
    return 0;
}

int statementList() {
    while (true) {
        if (!statement()) {
            break; // 如果不能匹配 statement，退出循环
        }
    }
    return 0;
}

int statement(){
    // 匹配语句
    // 前看符号防止回溯
    if(tokens[currentToken - 1].lexeme == "{"){
        // compound statement
        if(!compoundStmt()){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken - 1].lexeme == "if"){
        // section statement
        if(!selectionStmt()){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken - 1].lexeme == "while"){
        // iteration statement
        if(!iterationStmt()){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken - 1].lexeme == "return"){
        // return statement
        if(!returnStmt()){
            error("not a statement");
            return 1;
        }
    }
    else{
        // expression statement
        if(!expressionStmt()){
            error("not a statement");
            return 1;
        }
    }
    return 0;
}
int expressionStmt(){
    if(match(DELIMITER) && tokens[currentToken - 1].lexeme == ";"){
        return 0;
    }
    if(!expression()){
        return 1;
    }
    if(!match(DELIMITER) && tokens[currentToken - 1].lexeme == ";"){
        error("Expected ; after expression");
        return 1;
    }
    return 0;
}
int selectionStmt(){

}
int iterationStmt(){

}
int returnStmt(){

}
int expression(){

}
int var(){

}
int simpleExpression(){

}
int additiveExpression(){

}
int term(){

}
int factor(){

}
int call(){

}
int args(){

}
int argList(){

}

