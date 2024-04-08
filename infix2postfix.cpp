#include <iostream>
#include <stack>
#include <string>

using namespace std;

// 操作符优先级比较函数,乘除的优先级更高
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 中缀转后缀函数
string infixToPostfix(string infix) {
    stack<char> opStack; // 使用栈进行操作
    string postfix = "";

    for (char& c : infix) {
        if (c == ' ') continue;  // 忽略空格
        if (isdigit(c)) {
            postfix += c; // 数字直接加入后缀表达式
        } else if (c == '(') {
            opStack.push(c); // 左括号入栈
        } else if (c == ')') {
            // 右括号，将栈顶元素弹出并加入后缀表达式，直到遇到左括号
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop(); // 弹出左括号
        } else {
            // 操作符
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
    }

    // 将栈中剩余的操作符加入后缀表达式
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }

    return postfix;
}

int main() {
    string infixExpression;
    cout << "Enter infix expression: ";
    getline(cin, infixExpression);

    string postfixExpression = infixToPostfix(infixExpression);
    cout << "Postfix expression: " << postfixExpression << endl;

    getchar();
    return 0;
}
