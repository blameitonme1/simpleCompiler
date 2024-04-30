#ifndef LEXICAL_PARSER_H
#define LEXICAL_PARSER_H

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

enum tokenType {
    KEYWORD, //关键字
    IDENTIFIER, //标识符
    NUMBER, //常数
    OPERATOR, //操作符
    DELIMITER //分割符，比如冒号或者分号
};

struct Token {
    tokenType type; // Token 类型
    std::string lexeme; // Token 词素
};

// 全局变量声明
extern std::vector<Token> allTokens;
extern std::unordered_set<std::string> Keywords;
extern std::unordered_set<char> Operators_single;
extern std::unordered_set<std::string> Operators_double;
extern std::unordered_set<char> Delimiters;

// 函数声明
void debug_info(const Token& token);
void record_token_and_clear(Token& token);
void shoot_error();
void show_howmany_lines();
void tokenize(const std::string& input);

#endif
