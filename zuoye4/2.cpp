#include <iostream>
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()
#include <vector>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <fstream>   // For file output
#include <iomanip>
#include <cctype>    // For character handling

using namespace std;

// 函数声明
string generateFirstGradeProblem();
string generateSecondGradeProblem();
string generateThirdGradeProblem();
string generateFourthToSixthGradeProblem();
string generateGradeFourProblem();
string generateGradeFiveProblem();
double evaluateExpression(const string& expression);  // 简单表达式求值函数
string generateProblem(int maxNumber, const vector<char>& ops, bool useBrackets, bool useDecimal);
double evaluateExpression(const string& expression);
void printOrSaveProblem(const string& problem, ostream& out);



int main() {
    srand(time(0));  // 初始化随机数种子

    int numProblems, maxNumber;
    bool useBrackets, useDecimal, interactiveMode;
    vector<char> ops;
    char outputMode;
    string tempOp;

    cout << "请输入题目数量: ";
    cin >> numProblems;
    cout << "请选择运算符(+, -, *, /)，多个运算符用空格分隔: ";
    cin.ignore();
    getline(cin, tempOp);
    stringstream ss(tempOp);
    char op;
    while (ss >> op) {
        ops.push_back(op);
    }
    cout << "请输入最大数值: ";
    cin >> maxNumber;
    cout << "是否使用括号? (1 = 是, 0 = 否): ";
    cin >> useBrackets;
    cout << "是否使用小数? (1 = 是, 0 = 否): ";
    cin >> useDecimal;
    cout << "输出方式? (C = 控制台, F = 文件): ";
    cin >> outputMode;
    cout << "是否要在控制台进行答题? (1 = 是, 0 = 否): ";
    cin >> interactiveMode;

    ofstream outFile;
    if (outputMode == 'F') {
        outFile.open("problems.txt");
    }
    ostream& out = (outputMode == 'F' && outFile.is_open()) ? outFile : cout;

    for (int i = 0; i < numProblems; ++i) {
        string problem = generateProblem(maxNumber, ops, useBrackets, useDecimal);
        string fullProblem = "题目 " + to_string(i + 1) + ": " + problem + " = ?";
        printOrSaveProblem(fullProblem, out);

        if (interactiveMode) {
            double correctAnswer = evaluateExpression(problem);
            //cout << fullProblem << endl;
            cout << "请输入您的答案: ";
            double userAnswer;
            cin >> userAnswer;

            if (abs(userAnswer - correctAnswer) < 0.01) {  // 浮点数精度对比
                cout << "正确！" << endl;
            }
            else {
                cout << "错误，正确答案是: " << correctAnswer << endl;
            }
        }
    }

    if (outFile.is_open()) {
        outFile.close();
    }

    return 0;
}


// Utility function to generate random number within a range
int randomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}


string generateProblem(int maxNumber, const vector<char>& ops, bool useBrackets, bool useDecimal) {
    int numOperands = rand() % 4 + 2; // 生成 2 到 5 个操作数
    vector<double> numbers;
    vector<char> operators;

    // 生成操作数
    for (int i = 0; i < numOperands; i++) {
        double number = rand() % (maxNumber + 1);
        if (useDecimal) {
            number += (rand() % 100) / 100.0;
        }
        numbers.push_back(number);
    }

    // 选择运算符
    for (int i = 0; i < numOperands - 1; i++) {
        char op = ops[rand() % ops.size()];
        operators.push_back(op);
    }

    // 构建表达式
    stringstream expr;
    if (useBrackets && numOperands > 2) {
        // 为表达式添加随机括号
        int p1 = rand() % (numOperands - 1); // 选择一个位置开始括号
        int p2 = rand() % (numOperands - p1 - 1) + p1 + 1; // 选择一个位置结束括号
        expr << fixed << setprecision(2);

        for (int i = 0; i < numOperands; i++) {
            if (i == p1) expr << '(';
            expr << numbers[i];
            if (i == p2) expr << ')';
            if (i < numOperands - 1) expr << ' ' << operators[i] << ' ';
        }
    }
    else {
        // 不使用括号的简单构建
        expr << fixed << setprecision(2);
        for (int i = 0; i < numOperands; i++) {
            expr << numbers[i];
            if (i < numOperands - 1) expr << ' ' << operators[i] << ' ';
        }
    }

    return expr.str();
}
void printOrSaveProblem(const string& problem, ostream& out) {
    out << problem << endl;
}

// 简单表达式求值

// Function to perform arithmetic operations
double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw runtime_error("Division by zero.");
        return a / b;
    default: throw runtime_error("Unsupported operator encountered.");
    }
}

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to evaluate the expression given as string
double evaluateExpression(const string& expression) {
    stack<double> values;  // Stack to store double values
    stack<char> ops;       // Stack to store operators

    for (int i = 0; i < expression.length(); i++) {
        // Skip whitespaces
        if (isspace(expression[i]))
            continue;

        // If the current character is a digit or a decimal point, parse number
        if (isdigit(expression[i]) || expression[i] == '.') {
            stringstream ss;
            // Read number
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                ss << expression[i++];
            }
            double value;
            ss >> value;
            values.push(value);
            // Decrease i to balance the increment in the for loop
            i--;
        }
        else if (expression[i] == '(') {
            // If the token is a '(', push it to the stack for operators
            ops.push(expression[i]);
        }
        else if (expression[i] == ')') {
            // If the token is a ')', solve the entire bracket
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            // Pop the '(' from the stack
            ops.pop();
        }
        else {
            // Operator encountered
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            // Push current operator to stack
            ops.push(expression[i]);
        }
    }

    // Apply all remaining ops to values
    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    // Top of 'values' contains the result
    return values.top();
}