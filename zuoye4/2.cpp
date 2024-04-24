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

// ��������
string generateFirstGradeProblem();
string generateSecondGradeProblem();
string generateThirdGradeProblem();
string generateFourthToSixthGradeProblem();
string generateGradeFourProblem();
string generateGradeFiveProblem();
double evaluateExpression(const string& expression);  // �򵥱��ʽ��ֵ����
string generateProblem(int maxNumber, const vector<char>& ops, bool useBrackets, bool useDecimal);
double evaluateExpression(const string& expression);
void printOrSaveProblem(const string& problem, ostream& out);



int main() {
    srand(time(0));  // ��ʼ�����������

    int numProblems, maxNumber;
    bool useBrackets, useDecimal, interactiveMode;
    vector<char> ops;
    char outputMode;
    string tempOp;

    cout << "��������Ŀ����: ";
    cin >> numProblems;
    cout << "��ѡ�������(+, -, *, /)�����������ÿո�ָ�: ";
    cin.ignore();
    getline(cin, tempOp);
    stringstream ss(tempOp);
    char op;
    while (ss >> op) {
        ops.push_back(op);
    }
    cout << "�����������ֵ: ";
    cin >> maxNumber;
    cout << "�Ƿ�ʹ������? (1 = ��, 0 = ��): ";
    cin >> useBrackets;
    cout << "�Ƿ�ʹ��С��? (1 = ��, 0 = ��): ";
    cin >> useDecimal;
    cout << "�����ʽ? (C = ����̨, F = �ļ�): ";
    cin >> outputMode;
    cout << "�Ƿ�Ҫ�ڿ���̨���д���? (1 = ��, 0 = ��): ";
    cin >> interactiveMode;

    ofstream outFile;
    if (outputMode == 'F') {
        outFile.open("problems.txt");
    }
    ostream& out = (outputMode == 'F' && outFile.is_open()) ? outFile : cout;

    for (int i = 0; i < numProblems; ++i) {
        string problem = generateProblem(maxNumber, ops, useBrackets, useDecimal);
        string fullProblem = "��Ŀ " + to_string(i + 1) + ": " + problem + " = ?";
        printOrSaveProblem(fullProblem, out);

        if (interactiveMode) {
            double correctAnswer = evaluateExpression(problem);
            //cout << fullProblem << endl;
            cout << "���������Ĵ�: ";
            double userAnswer;
            cin >> userAnswer;

            if (abs(userAnswer - correctAnswer) < 0.01) {  // ���������ȶԱ�
                cout << "��ȷ��" << endl;
            }
            else {
                cout << "������ȷ����: " << correctAnswer << endl;
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
    int numOperands = rand() % 4 + 2; // ���� 2 �� 5 ��������
    vector<double> numbers;
    vector<char> operators;

    // ���ɲ�����
    for (int i = 0; i < numOperands; i++) {
        double number = rand() % (maxNumber + 1);
        if (useDecimal) {
            number += (rand() % 100) / 100.0;
        }
        numbers.push_back(number);
    }

    // ѡ�������
    for (int i = 0; i < numOperands - 1; i++) {
        char op = ops[rand() % ops.size()];
        operators.push_back(op);
    }

    // �������ʽ
    stringstream expr;
    if (useBrackets && numOperands > 2) {
        // Ϊ���ʽ����������
        int p1 = rand() % (numOperands - 1); // ѡ��һ��λ�ÿ�ʼ����
        int p2 = rand() % (numOperands - p1 - 1) + p1 + 1; // ѡ��һ��λ�ý�������
        expr << fixed << setprecision(2);

        for (int i = 0; i < numOperands; i++) {
            if (i == p1) expr << '(';
            expr << numbers[i];
            if (i == p2) expr << ')';
            if (i < numOperands - 1) expr << ' ' << operators[i] << ' ';
        }
    }
    else {
        // ��ʹ�����ŵļ򵥹���
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

// �򵥱��ʽ��ֵ

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