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

using namespace std;

// 函数声明
string generateFirstGradeProblem();
string generateSecondGradeProblem();
string generateThirdGradeProblem();
string generateFourthToSixthGradeProblem();
string generateGradeFourProblem();
string generateGradeFiveProblem();
double evaluateExpression(const string& expression);  // 简单表达式求值函数

int main() {
    srand(time(0));  // 初始化随机数种子

    int grade, numProblems;
    cout << "请输入年级（1-6）和题目数量: ";
    cin >> grade >> numProblems;
    cin.ignore(); // 清除缓冲区，准备读取字符串

    vector<string> problems;

    for (int i = 0; i < numProblems; ++i) {
        string problem;
        switch (grade) {
        case 1:
            problem = generateFirstGradeProblem();
            break;
        case 2:
            problem = generateSecondGradeProblem();
            break;
        case 3:
            problem = generateThirdGradeProblem();
            break;
        case 4:
            problem = generateGradeFourProblem();
            break;
        case 5:
            problem = generateGradeFiveProblem();
            break;
        case 6:
            problem = generateFourthToSixthGradeProblem();
            break;
        default:
            cout << "无效的年级输入！";
            return 1;
        }
        problems.push_back(problem);
        double correctAnswer = evaluateExpression(problem);
        if (correctAnswer < 0)
        {
            i--;
            continue;
        }
        cout << "题目 " << (i + 1) << ": " << problem << " = ?\n";
        string answer;
        getline(cin, answer);
        double userAnswer = stod(answer);
        

        if (abs(userAnswer - correctAnswer) < 1e-4) {
            cout << "正确！\n";
        }
        else {
            cout << "错误，正确答案是: " << correctAnswer << "\n";
        }
    }

    return 0;
}
// 一年级：20以内加法或减法
string generateFirstGradeProblem() {
    int num1 = rand() % 21;
    int num2 = rand() % (21 - num1);  // 保证结果不大于20
    char op = (rand() % 2 == 0) ? '+' : '-';
    if (op == '-' && num1 < num2) swap(num1, num2);  // 保证非负结果
    return to_string(num1) + " " + op + " " + to_string(num2);
}

// 二年级：50以内加减法
string generateSecondGradeProblem() {
    int num1 = rand() % 51;
    int num2 = rand() % 51;
    char op = (rand() % 2 == 0) ? '+' : '-';
    if (op == '-' && num1 < num2) swap(num1, num2);
    return to_string(num1) + " " + op + " " + to_string(num2);
}

// 三年级：1000以内加减乘
string generateThirdGradeProblem() {
    int num1 = rand() % 1001;
    int num2 = rand() % 1001;
    char ops[3] = { '+', '-', '*' };
    char op = ops[rand() % 3];
    return to_string(num1) + " " + op + " " + to_string(num2);
}

// 四至六年级：复杂表达式与随机括号
string generateFourthToSixthGradeProblem() {
    int num1 = rand() % 101;
    int num2 = rand() % 101;
    int num3 = rand() % 101;
    int num4 = rand() % 101;
    char ops[4] = { '+', '-', '*', '/' };
    char op1 = ops[rand() % 4];
    char op2 = ops[rand() % 4];
    char op3 = ops[rand() % 4];

    // 随机决定括号的位置
    int bracketPattern = rand() % 3;
    string expression;

    switch (bracketPattern) {
    case 0:
        expression = "(" + to_string(num1) + " " + op1 + " " + to_string(num2) + ") " + op2 + " (" + to_string(num3) + " " + op3 + " " + to_string(num4) + ")";
        break;
    case 1:
        expression = "(" + to_string(num1) + " " + op1 + " (" + to_string(num2) + " " + op2 + " " + to_string(num3) + ")) " + op3 + " " + to_string(num4);
        break;
    case 2:
        expression = to_string(num1) + " " + op1 + " ((" + to_string(num2) + " " + op2 + " " + to_string(num3) + ") " + op3 + " " + to_string(num4) + ")";
        break;
    }

    return expression;
}

// Utility function to generate random number within a range
int randomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to generate problems for Grade 4 (results must be integers)
string generateGradeFourProblem() {
    int num1 = randomInRange(1, 50);
    int num2 = randomInRange(1, 50);
    int num3 = randomInRange(1, 50);
    int num4 = randomInRange(1, 50);
    vector<char> ops = { '+', '-', '*', '/' };

    // Ensuring integer results
    char op1 = ops[randomInRange(0, 3)];
    char op2 = ops[randomInRange(0, 3)];
    char op3 = ops[randomInRange(0, 3)];

    if (op1 == '/') num1 *= num2;  // Adjust to ensure integer division
    if (op2 == '/') num2 *= num3;  // Adjust to ensure integer division
    if (op2 == '/') num4 *= num3;  // Adjust to ensure integer division

    return to_string(num1) + " " + op1 + " " + to_string(num2) + " " + op2 + " " + to_string(num3) + " " + op3 + " " + to_string(num4);
}

// Function to generate problems for Grade 5 (division must not result in infinite decimals)
string generateGradeFiveProblem() {
    int num1 = randomInRange(1, 100);
    int num2 = randomInRange(1, 100);
    int num3 = randomInRange(1, 100);
    vector<char> ops = { '+', '-', '*', '/' };

    // Adjusting numbers to avoid infinite decimal in division
    char op1 = ops[randomInRange(0, 3)];
    char op2 = ops[randomInRange(0, 3)];
    char op3 = ops[randomInRange(0, 3)];

    if (op1 == '/') num1 *= num2;
    if (op2 == '/') num2 *= num3;

    string expression = "(" + to_string(num1) + " " + op1 + " " + to_string(num2) + ")";
    expression = expression +" " + op2 + " ";
    expression += to_string(num3) + " " + op3;
    return expression;
}

// 简单表达式求值

// Function to perform arithmetic operations.
double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b != 0 ? a / b : throw invalid_argument("Division by zero.");
    default: throw invalid_argument("Unsupported operator encountered.");
    }
}

// Function to return precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to perform the actual calculation
double evaluateExpression(const string& tokens) {
    stack<double> values;
    stack<char> ops;

    for (int i = 0; i < tokens.length(); i++) {
        // Current token is a whitespace, skip it.
        if (tokens[i] == ' ')
            continue;

        // Current token is a number, push it to stack for numbers.
        if (isdigit(tokens[i])) {
            double val = 0;
            // There may be more than one digits in the number.
            while (i < tokens.length() && isdigit(tokens[i])) {
                val = (val * 10) + (tokens[i] - '0');
                i++;
            }
            values.push(val);
            // Since the loop advances 'i', we decrease it to compensate for the outer for loop increment.
            i--;
        }

        // Current token is an opening brace, push it to 'ops'
        else if (tokens[i] == '(') {
            ops.push(tokens[i]);
        }

        // Closing brace encountered, solve entire brace
        else if (tokens[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            // Pop opening brace.
            if (!ops.empty())
                ops.pop();
        }

        // Current token is an operator.
        else {
            // While top of 'ops' has same or greater precedence to current token, which is an operator. Apply operator on top of 'ops' to top two elements in values stack
            while (!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])) {
                double val2 = values.top();
                values.pop();
                double val1 = values.top();
                values.pop();
                char op = ops.top();
                ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            // Push current token to 'ops'.
            ops.push(tokens[i]);
        }
    }

    // Entire expression has been parsed at this point, apply remaining ops to remaining values.
    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();
        double val1 = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    // Top of 'values' contains result, return it.
    return values.top();
}