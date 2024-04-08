#include<iostream>
#include<string>
#include<cctype>
#include <vector>
#include <unordered_set>
using namespace std;

enum tokenType {
    KEYWORD, //关键字
    IDENTIFIER, //标识符
    NUMBER, //常数
    OPERATOR, // 操作符
    DELIMITER // 分割符，比如冒号或者分号
};

struct Token // 分析得到的数据结构，传递给语法分析器
{
    tokenType type; // key
    string lexeme; // value
};

vector<Token>allTokens;
unordered_set<string>Keywords{"int", "float", "break", "case", "char",
"const", "continue", "default", "do", "while", "for", "double", "else", "enum",
"extern", "for", "goto", "if", "inline", "long", "return", "short",
"signed", "unsigned", "sizeof", "static", "register", "struct", "switch",
"typedef", "union", "void", "main"};

void tokenize(const string& input){
    Token token;
    int i = 0; // 当前字符下标
    int length = input.length();

    for(; i < length; ++i){
        // 忽略空白
        if(isspace(input[i])){
            continue;
        }

        // 匹配标识符或者关键字
        if(isalpha(input[i])){
            int j = i;
            while(isalnum(input[j]) || input[j] == '_'){
                // 标识符，字母数字或者下划线
                token.lexeme.push_back(input[j++]);
            }

            // 判断是否为关键字
            if(Keywords.count(token.lexeme)){
                // 读取的是关键字
                token.type = KEYWORD;
            } else {
                token.type = IDENTIFIER;
            }
        }


    }
}