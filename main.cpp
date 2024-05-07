#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include "lexical_parser.h"
#include "syntax_parser.h"
using namespace std;

int main(){
    // 获取代码
    ifstream file("./srccode.txt");
    if(!file.is_open()){
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string content = "";
    string line = "";
    while(getline(file, line)){
        content += line + "\n";
    }
    cout << "content:\n" << content << endl;
    // 词法分析
    tokenize(content);
    show_howmany_lines();
    for(int i = 0; i < tokens.size(); i++){
        cout << "i: " << i << " " << tokens[i].type << " " << tokens[i].lexeme << endl;
    }
    // 语法分析
    if(program() == 0){
        cout << "Success!" << endl;
    }
    else{
        cout << "Failed!" << endl;
    }
    getchar();
    return 0;
}