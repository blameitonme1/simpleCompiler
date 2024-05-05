#include<iostream>
#include<fstream> 
#include<string>
#include<cctype>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "lexical_parser.h"
using namespace std;


unordered_map<int, string> typeNum2typeName {{0,"keyword"},{1,"identifier"},
{2,"number"},{3,"operator"},{4,"delimiter"}};

vector<Token>tokens;
unordered_set<string>Keywords{"int", "float", "break", "case", "char",
"const", "continue", "default", "do", "while", "for", "double", "else", "enum",
"extern", "for", "goto", "if", "inline", "long", "return", "short",
"signed", "unsigned", "sizeof", "static", "register", "struct", "switch",
"typedef", "union", "void"};
unordered_set<char>Operators_single{'+', '-', '/', '*','=','<','>','&','|','%','^'}; // 单字运算符,考虑位操作
unordered_set<string>Operators_double{">=","<=","==", "!=","&&","||",">>","<<","+=","-=",
"*=","/=","%=","&=","|=","^="}; // 双字运算符,考虑位操作
unordered_set<char>Delimiters{';', '(', ')', '{', '}',',','[',']'};

size_t row = 0; // 记录行数，方便精准定位

void debug_info(const Token& token){
    // 调试信息
    cout << "key value pair: < '" << token.lexeme << "' , " << typeNum2typeName[token.type] <<  ">" << endl;
}

void record_token_and_clear(Token& token){
    // 全局记录token并且清除token的lexeme
    tokens.push_back(token);
    token.lexeme.clear();
}

void shoot_error(){
    // 报错
    cout << "error occurred in line " << row << endl;
}

void show_howmany_lines(){
    cout << "All " << row << " lines of code" << endl;
}

void tokenize(const string& input){
    Token token;
    int i = 0; // 当前字符下标
    int length = input.length();
    while(i < length){
        // 忽略空白以及换行符，制表符
        if(isspace(input[i]) || input[i] == '\n' || input[i] == '\t'){
            if(input[i] == '\n'){
                ++row;
            }
            ++i;
            continue;
        }
        // 忽略注释
        if(i + 1 < length && input[i] == '/' && input[i + 1] == '*'){
            i += 2;
            while(i + 1 < length && !(input[i] == '*' && input[i + 1] == '/')){
                ++i;
            }
            i += 2;
            continue;
        }
        // 匹配标识符或者关键字
        if(isalpha(input[i])){
            while(i < length && (isalnum(input[i]) || input[i] == '_')){
                // 标识符，字母数字或者下划线
                token.lexeme.push_back(input[i++]);
            }
            --i;
            // 判断是否为关键字
            if(Keywords.count(token.lexeme)){
                // 读取的是关键字
                token.type = KEYWORD;
            } else {
                token.type = IDENTIFIER;
            }
        }
        else if(isdigit(input[i])){
            // 匹配数字
            while(i < length && (isdigit(input[i]) || input[i] == '.')){
                // 最长匹配数字
                token.lexeme.push_back(input[i++]);
            }
            --i;
            token.type = NUMBER;
        }
        else if(Operators_single.count(input[i])){   
            // 先匹配单字  
            token.lexeme = input[i];
            token.type = OPERATOR;
            string temp = "";
            temp.push_back(input[i]);
            if(i + 1 >= length){
                // 运算符在最后了
                shoot_error();
                return ;
            }
            temp.push_back(input[i+1]);
            if(Operators_double.count(temp)){
                token.lexeme = temp;
                i++;
            }
        }
        else if(Delimiters.count(input[i])){
            token.lexeme = input[i];
            token.type = DELIMITER;
        }
        else{
            shoot_error();
        }

        debug_info(token);
        record_token_and_clear(token);
        ++i;
    }
}
