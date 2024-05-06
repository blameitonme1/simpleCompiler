#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "lexical_parser.h"
#include "syntax_parser.h"



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
        currentToken--;
        if(typeSpecifier() == 1){
            return 1;
        } //识别类型
        if (match(IDENTIFIER)) {
            // Move token consumption logic into the specific function branches
            if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
                // Function declaration
                // std::cout << "here" << std::endl;
                currentToken -= 3; // Unconsume ID
                funDeclaration();
            } else {
                // Variable declaration
                if(varDeclaration() == 1){
                    return 1;
                }
            }
        } else {
            error("Expected identifier after type specifier."); // 应给出标识符
            return 1;
        }
    } else {
        std::cout << currentToken << std::endl;
        error("Expected type specifier in declaration."); // 声明应该报类型
        return 1;
    }
    return 0;
}

int varDeclaration() {
    std::cout << "var declaration " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
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
            error("Expected ; after value declaration");
            return 1;
        }
    }
    return 0;
}

int typeSpecifier(){
    std::cout << " type specifier " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(!match(KEYWORD)){
        error("Expected type specifier (int or void)");
        return 1;
    }
    if(tokens[currentToken - 1].lexeme != "int" && tokens[currentToken - 1].lexeme != "void"){
        error("invalid type");
        return 1;
    }
    return 0;
}

int funDeclaration() {
    // 匹配函数声明
    std::cout << "fun declaration " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(typeSpecifier() == 1){
        return 1;
    }
    if (match(IDENTIFIER)) {
        if (match(DELIMITER) && tokens[currentToken - 1].lexeme == "(") {
            // Function declaration with parameters
            if(params() == 1){
                return 1;
            }
            if (match(DELIMITER) && tokens[currentToken - 1].lexeme == ")") {
                // Function declaration with no parameters
                if(compoundStmt() == 1){
                    // 复合语句
                    return 1;
                }
            } else {
                error("Expected ')' after parameter list."); // 没匹配到右括号
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
    return 0;
}

int params(){
    // 匹配参数列表
    // std::cout << tokens[currentToken].lexeme << std::endl;
    std::cout << "param " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(tokens[currentToken].lexeme == "void"){
        // std::cout << "retuing" << std::endl;
        currentToken++;
        return 0; // 前看符号，匹配成功
    }
    if(paramList() == 1){
        return 1;
    }
    return 0;
}

int paramList(){
    // 匹配有实际参数的时候的参数列表
    std::cout << "paramlist " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(param() == 1){
        return 1;
    }
    while(match(DELIMITER) && tokens[currentToken - 1].lexeme == ","){
        if(param() == 1){
            return 1;
        }
    }
    if(tokens[currentToken - 1].lexeme == ")"){
        currentToken--;
    }
    return 0;
}

int param(){
    // 匹配单个参数
    // std::cout << tokens[currentToken].lexeme << std::endl;
    std::cout << "param " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(typeSpecifier() == 1){
        return 1;
    }
    if(!match(IDENTIFIER)){
        error("expect identifier for parameter");
        return 1;
    }
    if(match(DELIMITER)){
        if(tokens[currentToken - 1].lexeme == "[") {

            if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "]"){
                // 差一个右括号匹配成功
                error("Expected ] after [");
                return 1;
            }
        }
        else if(tokens[currentToken - 1].lexeme == ","){
            currentToken--; // 回看
        }
        else if(tokens[currentToken - 1].lexeme == ")"){
            currentToken--;
            return 0;
        }
        else{
            return 1;
        }
    }
    return 0;
}

int compoundStmt(){
    // 匹配复合语句
    std::cout << "compund stmt " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "{"){
        error("Expected { at the begining of compound statement");
        return 1;
    }
    if(localDeclarations() == 1){
        // 打印错误信息
        error("not a compound statement, no loal declaration");
    }
    if(statementList() == 1){
        // 打印错误信息
        error("not a compound statement, no statement list");
    }
    // 匹配右括号
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "}"){
        error("Expected } at the end of compound statement");
        return 1;
    }
    // 打印当前token的信息
    debug_info(tokens[currentToken]);
    return 0;
}

int localDeclarations() {
    std::cout << "local declaration " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    int tokenNum = currentToken;
    bool x = false;
    while (true) {
        tokenNum = currentToken;
        if (varDeclaration() == 1) {
            x = true;
            break; // 如果不能匹配 var-declaration，退出循环
        }
    }
    if(x){
        std::cout << "back, aborting local declaration" << std::endl;
        currentToken = tokenNum; // 复原
    }
    return 0;
}

int statementList() {
    std::cout << "statement list " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    int tokenNum = currentToken;
    bool x = false;
    while (true) {
        tokenNum = currentToken;
        if (statement() == 1) {
            x = true;
            break; // 如果不能匹配 statement，退出循环
        }
    }
    if(x){
        currentToken = tokenNum; // 复原
    }
    return 0;
}

int statement(){
    // 匹配语句
    // 前看符号防止回溯
    std::cout << "statement " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(tokens[currentToken].lexeme == "{"){
        // compound statement
        if(compoundStmt() == 1){
            error("not a compound statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "if"){
        // selection statement
        if(selectionStmt() == 1){
            error("not a selection statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "while"){
        // iteration statement
        if(iterationStmt() == 1){
            error("not a iteration statement");
            return 1;
        }
    }
    else if(tokens[currentToken].lexeme == "return"){
        // return statement
        if(returnStmt() == 1){
            error("not a return sstatement");
            return 1;
        }
    }
    else{
        // expression statement
        if(expressionStmt() == 1){
            error("not a expression statement");
            return 1;
        }
    }
    return 0;
}

int expressionStmt(){
    std::cout << "expression statement " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    std::cout << "expression STMT " << tokens[currentToken].lexeme << std::endl;
    if(match(DELIMITER) && tokens[currentToken - 1].lexeme == ";"){
        return 0;
    }
    // std::cout << "expression" << std::endl;
    if(expression() == 1){
        return 1;
    }
    // std::cout << "expression" << std::endl;
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != ";"){
        error("Expected ; after expression statement");
        return 1;
    }
    return 0;
}

int selectionStmt(){
    // 选择语句
    std::cout << "selection statement " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
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
        if(statement() == 1){
            return 1;
        }
    }
    return 0;
}
int iterationStmt(){
    // 匹配循环语句
    std::cout << "iteration declaration " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "while"){
        error("Expected while");
        return 1;
    }
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != "("){
        error("Expected ( in iteration");
        return 1;
    }
    if(expression() == 1){
        return 1;
    }
    if(!match(KEYWORD) || tokens[currentToken - 1].lexeme != ")"){
        error("Expected ) in iteration");
        return 1;
    }
    if(statement() == 1){
        return 1;
    }
    return 0;
}
int returnStmt(){
    // 匹配返回语句
    std::cout << "return statement " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
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
    return 0;
}
int expression(){
    // 匹配通常表达式
    std::cout << "expression " << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    while(tokens[currentToken + 1].lexeme == "="){
        // 先匹配 若干个 'var='
        if(var() == 1){
            return 1;
        }
        if(!match(OPERATOR) || (tokens[currentToken - 1].lexeme != "=")){
            std::cout << tokens[currentToken - 1].lexeme << std::endl;
            error("Expected = or ==");
            return 1;
        }
        // 注意是这里出现bug!!!
    }
    std::cout << "debug expression calling simple expression " << tokens[currentToken].lexeme << std::endl;
    if(simpleExpression() == 1){
        // 最后匹配一个简单表达式
        std::cout << "simple error " << tokens[currentToken].lexeme << std::endl;
        error("Lack simple expression after expression");
        return 1;
    }
    return 0;
}
int var(){
    // 匹配变量
    std::cout << "var: " << tokens[currentToken].lexeme << std::endl;
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
    std::cout << "simple expression" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    // 匹配一条简单表达式
    if(additiveExpression() == 1){
        std::cout << "first addExe error " << tokens[currentToken].lexeme << std::endl;
        return 1;
    } 
    std::string forward = tokens[currentToken].lexeme;    
    // std::cout << forward << std::endl;  
    if(forward == "<=" || forward == "<" || forward == ">" || forward == ">=" || forward == "==" || forward == "!="){
        if(relop() == 1){
            return 1;
        }
        if(additiveExpression() == 1){
            error("No additive expresision after relop");
            return 1;
        } 
    }
    return 0;
}

int relop(){
    std::cout << "relop" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
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
    std::cout << "add Exp" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(term() == 1){
        std::cout << "first term error" << tokens[currentToken].lexeme << std::endl;
        return 1;
    }
    while (tokens[currentToken].lexeme == "+" || tokens[currentToken].lexeme == "-"){
        currentToken ++;
        if(term() == 1){
                    std::cout << "second term error " << tokens[currentToken].lexeme << std::endl;
            return 1;
        }
    }
    return 0;
    
}
int term(){
    // term
    std::cout << "term" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(factor() == 1){
        std::cout << "first factor error term" << tokens[currentToken].lexeme << std::endl;
        return 1;
    }
    while(tokens[currentToken].lexeme == "*" || tokens[currentToken].lexeme == "/"){
        currentToken ++;
        if(factor() == 1){
                    std::cout << "afterward factor error " << tokens[currentToken].lexeme << std::endl;
            return 1;
        }
    }
    return 0;
}
int factor(){
    std::cout << "factor" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(match(NUMBER)){
        // std::cout << "number!" << std::endl;
        return 0;
    }
    // std::cout << tokens[currentToken].lexeme << std::endl;
    // std::cout << " factor" << std::endl;
    if(tokens[currentToken].lexeme == "("){
        // 匹配第一项
        currentToken ++;
        if(expression() == 1){
            return 1;
        }
        return 0;
    }
    // 前看两个符号防止回溯
    else if(tokens[currentToken + 1].lexeme == "("){
        // 匹配call
        if(call() == 1){
            return 1;
        }
        std::cout << "call finish factor:" << tokens[currentToken].lexeme << std::endl;
        return 0;
    }
    else {
        // var
        std::cout << "calling var from factor" << std::endl;
        if(var() == 1){
            return 1;
        }
        return 0;
    }
    return 0;
}
int call(){
    std::cout << "call:" << tokens[currentToken].lexeme << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    if(!match(IDENTIFIER)){
        error("Expected ID");
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != "("){
        error("Expected (");
        return 1;
    }
    if(tokens[currentToken].lexeme == ")"){
        std::cout << "call finish:" << tokens[currentToken].lexeme << std::endl;
        currentToken++;
        return 0;
    }
    if(args() == 1){
        return 1;
    }
    if(!match(DELIMITER) || tokens[currentToken - 1].lexeme != ")"){
        error("Expected )");
        return 1;
    }
    std::cout << "call finish last:" << tokens[currentToken].lexeme << std::endl;
    return 0;
}
int args(){
    std::cout << "arg" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
    int tokenNum = currentToken;
    std::cout << " args: " << tokens[currentToken].lexeme << std::endl;
    if(argList() == 1){
        currentToken = tokenNum; // 回归原来的count
    }
    return 0;
}
int argList(){
    std::cout << "arglist" << std::endl;
    std::cout << tokens[currentToken].lexeme << std::endl;
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

