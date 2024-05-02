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
int relop();
int args();
int argList();

// Sample function implementations
int program() {
    if(declarationList() == 1){
        return 1;
    }
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
        if(declaration() == 1){
            return 1;
        }
    } while (currentToken < tokens.size());
    return 0;
}

int declaration() {
    if (match(KEYWORD)) {
        if(typeSpecifier() == 1){
            return 1;
        } //识别类型
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
                if(varDeclaration() == 1){
                    return 1;
                }
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
    if(typeSpecifier() == 1){
        return 1;
    } // 解析变量类型
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
    if(typeSpecifier() == 1){
        return 1;
    }
    if (match(IDENTIFIER)) {
        if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
            // Function declaration with parameters
            if(params() == 1){
                return 1;
            }
        } else {
            error("Expected '(' after function name."); // 没匹配到左括号
            return 1;
        }
    }
    else {
        error("Expected identifier in function declaration.");
        return 1;
    }
    if(compoundStmt() == 1){
        return 1;
    } // 匹配跟着的复合语句
    return 0;
}

int params(){
    // 匹配参数列表
    if(tokens[currentToken - 1].lexeme == "void"){
        return ; // 前看符号，匹配成功
    }
    if(paramList() == 1){
        return 1;
    }
    return 0;
}

int paramList(){
    // 匹配有实际参数的时候的参数列表
    if(param() == 1){
        return 1;
    }
    while(match(DELIMITER) && tokens[currentToken - 1].lexeme == ","){
        if(param() == 1){
            return 1;
        }
    }
    return 0;
}

int param(){
    // 匹配单个参数
    if(typeSpecifier() == 1){
        return 1;
    }
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
        if (varDeclaration() == 1) {
            break; // 如果不能匹配 var-declaration，退出循环
        }
    }
    return 0;
}

int statementList() {
    while (true) {
        if (statement() == 1) {
            break; // 如果不能匹配 statement，退出循环
        }
    }
    return 0;
}

int statement(){
    // 匹配语句
    // 前看符号防止回溯
    if(tokens[currentToken].lexeme == "{"){
        // compound statement
        if(compoundStmt() == 1){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "if"){
        // selection statement
        if(selectionStmt() == 1){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "while"){
        // iteration statement
        if(iterationStmt() == 1){
            error("not a statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "return"){
        // return statement
        if(returnStmt() == 1){
            error("not a statement");
            return 1;
        }
    }
    else{
        // expression statement
        if(expressionStmt() == 1){
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
    if(expression() == 1){
        return 1;
    }
    if(!match(DELIMITER) && tokens[currentToken - 1].lexeme == ";"){
        error("Expected ; after expression");
        return 1;
    }
    return 0;
}

int selectionStmt(){
    // 选择语句
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "if"){
        error("Expectd if in the begining of selection statement");
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "("){
        error("Expected left parent");
        return 1;
    }
    expression();
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != ")"){
        error("Expected right parent");
        return 1;
    }
    if(statement() == 1){
        return 1;
    }
    // 前看防止悬挂else
    if(tokens[currentToken].lexeme == "else"){
        // 匹配else语句
        currentToken++;
        if(!statement()){
            return 1;
        }
    }
    return 0;
}
int iterationStmt(){
    // 匹配循环语句
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "while"){
        error("Expected while");
        return 1;
    }
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "("){
        error("Expected (");
        return 1;
    }
    if(expression() == 1){
        return 1;
    }
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != ")"){
        error("Expected )");
        return 1;
    }
    if(statement() == 1){
        return 1;
    }

}
int returnStmt(){
    // 匹配返回语句
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "return"){
        error("Expected return");
        return 1;
    }
    // 前看防止回溯
    if(tokens[currentToken].lexeme == ";"){
        currentToken++;
        return 0;
    }
    if(expression() == 1){
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != ";"){
        return 1;
    }
}
int expression(){
    // 匹配通常表达式
    // 前看（可知是simple expression
    if(tokens[currentToken].lexeme == "("){
        if(simpleExpression() == 1){
            return 1;
        }
        else{
            // 需要提前返回
            return 0;
        }
    }
    if(var() == 1){
        return 1;
    }
    if(!match(OPERATOR) || tokens[currentToken - 1].lexeme != "="){
        error("Expected =");
        return 1;
    }
    if(expression() == 1){
        // 递归匹配
        return 1;
    }
    return 0;
}
int var(){
    // 匹配变量
    if(!match(IDENTIFIER)){
        error("Expected ID");
        return 1;
    }
    if(tokens[currentToken].lexeme == "["){
        currentToken ++;
        if(expression() == 1){
            return 1;
        }
        if(tokens[currentToken].lexeme != "]"){
            error("Expectd ]");
            return 1;
        }
        currentToken ++; // 匹配成功，看下一个字符
    }
    return 0;
}

int simpleExpression(){
    // 匹配一条简单表达式
    if(additiveExpression() == 1){
        return 1;
    } 
    std::string forward = tokens[currentToken].lexeme;      
    if(forward == "<=" || forward == "<" || forward == ">" || forward == ">=" || forward == "==" || forward == "!="){
        if(relop() == 1){
            return 1;
        }
        if(additiveExpression() == 1){
            return 1;
        } 
    }
    return 0;
}

int relop(){
    std::string forward = tokens[currentToken].lexeme;  
    if(forward == "<=" || forward == "<" || forward == ">" || forward == ">=" || forward == "==" || forward == "!="){
        currentToken++; // 只在这里加
        return 0;
    }
    else{
        error("Wrong relop");
        return 1;
    }
}

int additiveExpression(){
    // 加法表达式
    if(term() == 1){
        return 1;
    }
    while (tokens[currentToken].lexeme == "+" || tokens[currentToken].lexeme == "-"){
        currentToken ++;
        if(term() == 1){
            return 1;
        }
    }
    return 0;
    
}
int term(){
    // term
    if(factor() == 1){
        return 1;
    }
    while(tokens[currentToken].lexeme == "*" || tokens[currentToken].lexeme == "/"){
        currentToken ++;
        if(factor() == 1){
            return 1;
        }
    }
    return 0;
}
int factor(){
    if(match(NUMBER)){
        return 0;
    }
    if(tokens[currentToken].lexeme == "("){
        // 匹配第一项
        currentToken ++;
        if(expression() == 1){
            return 1;
        }
        return 0;
    }
    // 前看两个符号防止回溯
    if(tokens[currentToken + 1].lexeme == "("){
        // 匹配call
        if(call() == 1){
            return 1;
        }
        return 0;
    }
    else{
        // var
        if(var() == 1){
            return 1;
        }
        return 0;
    }
    return 0;
}
int call(){
    if(!match(IDENTIFIER)){
        error("Expected ID");
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "("){
        error("Expected (");
        return 1;
    }
    if(args() == 1){
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != ")"){
        error("Expected )");
        return 1;
    }
    return 0;
}
int args(){
    int tokenNum = currentToken;
    if(argList() == 1){
        currentToken = tokenNum; // 回归原来的count
    }
    return 0;
}
int argList(){
    if(expression() == 1){
        return 1;
    }
    while(tokens[currentToken].lexeme == ","){
        currentToken ++;
        if(expression() == 1){
            return 1;
        }
    }
    return 0;
}

