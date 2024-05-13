#include <stdio.h>
#include "parser.tab.h"

extern FILE *yyin; // Flex 生成的全局变量，用于指定输入文件流

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("fopen");
        return 1;
    }

    yyin = file; // 将输入文件流指定给 Flex 的全局变量 yyin

    yyparse(); // 调用语法分析器进行解析

    fclose(file);
    return 0;
}

// yyerror 函数在 parser_tab.c 中已经定义
