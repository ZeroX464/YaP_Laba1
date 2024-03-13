#include <iostream>
#include <stack>
using namespace std;

int priority(char operation) {
    if (operation == '^')
        return 3;
    else if (operation == '*' || operation == '/')
        return 2;
    else if (operation == '+' || operation == '-')
        return 1;
    else
        return -1;  //��� sin, cos � ������
}

string toPolishNotation(string commonExpression) {
    stack <char> operationStack;
    string PolishNotation;
    for (char ch : commonExpression) {
        if (isalnum(ch)) {  //������� (����� ��� �����)
            PolishNotation += ch;
        }
        else if (ch == '(') {
            operationStack.push(ch);
        }
        else if (ch == ')') {
            while (!operationStack.empty() && operationStack.top() != '(') { //��� �������� �� ( ����������� � ����� �������� ������
                PolishNotation += operationStack.top();
                operationStack.pop();
            }
            operationStack.pop();  //�������� (
        }
        else {  //��������
            while (!operationStack.empty() && priority(ch) <= priority(operationStack.top())) { //���� ��������� �������� ����, ��� � �����, �� �������� �� ����� ��������� � ������
                PolishNotation += operationStack.top();
                operationStack.pop();
            }
            operationStack.push(ch);
        }
    }
    while (!operationStack.empty()) { //������� ������� ����� � ����� �������� ������ ����� ���������� ������ commonExpression
        PolishNotation += operationStack.top();
        operationStack.pop();
    }
    return PolishNotation;
}

int calculatePolishNotation(string PolishNotation) {
    stack <int> operandStack;
    string number;
    bool multi_digit = false;

    for (char ch : PolishNotation) {
        if (isdigit(ch)) {  //��� �����
            number += ch;
        }
        else {  //��� ��������
            //operandStack.push(strtol(number, NULL, 10));
            number = "";
            int operand2 = operandStack.top();
            operandStack.pop();
            int operand1 = operandStack.top();
            operandStack.pop();

            switch (ch) {
            case '+':
                operandStack.push(operand1 + operand2);
                break;
            case '-':
                operandStack.push(operand1 - operand2);
                break;
            case '*':
                operandStack.push(operand1 * operand2);
                break;
            case '/':
                operandStack.push(operand1 / operand2);
                break;
            case '^':
                operandStack.push(pow(operand1, operand2));
                break;
            default:
                cerr << "����� �������� ����: '" << ch << "'" << endl;
                return 0;
            }
        }
    }
    if (!operandStack.empty()) {
        return operandStack.top();
    }
    else {
        cerr << "���� �������� ������" << endl;
        return 0;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    //string commonExpression = "5+(3*(-5))/(sin(50)^cos(5))";
    string commonExpression = "2+2*2^(-4)";
    string PolishNotation = toPolishNotation(commonExpression);
    int result = calculatePolishNotation(PolishNotation);
    cout << "������� ������ � ����������� �����: " << commonExpression << endl;
    cout << "���� ������ � �������� �������: " << PolishNotation << endl;
    cout << "��������� ����������: " << result << endl;
    return 0;
}