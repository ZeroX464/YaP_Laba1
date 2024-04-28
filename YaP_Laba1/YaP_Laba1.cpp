#include <iostream>
#include <stack>
#include <algorithm>
#define pi 3.14159265358979323846
using namespace std;

int priority(char operation) {
    if (operation == '~' || operation == 's' || operation == 'c')
        return 4;
    else if (operation == '^')
        return 3;
    else if (operation == '*' || operation == '/')
        return 2;
    else if (operation == '+' || operation == '-')
        return 1;
    else
        return -1;  //��� sin, cos � ������
}

string toPolishNotation(const string commonExpression) {
    stack <char> operationStack;
    string PolishNotation;

    auto putTopFromStackToNotation = [&]() { // & - �������� ���������� �� ������
        PolishNotation += operationStack.top();
        if (operationStack.top() == 's') { PolishNotation += "in"; }
        if (operationStack.top() == 'c') { PolishNotation += "os"; }
        operationStack.pop();
        };

    for (int i = 0; i < commonExpression.length(); i++) {
        char ch = commonExpression[i];
        if (isdigit(ch)) {  //������� (�����)
            PolishNotation += ch;
        }
        else if (ch == '(') {
            operationStack.push(ch);
        }
        else if (ch == ')') {
            while (!operationStack.empty() && operationStack.top() != '(') { //��� �������� �� ( ����������� � ����� �������� ������
                putTopFromStackToNotation();
            }
            operationStack.pop();  //�������� (
        }
        else if (ch == '-' && (i == 0 || commonExpression[i - 1] == '(' || !isdigit(commonExpression[i - 1]))) { 
            operationStack.push('~'); // ������� �����
        }
        else { //��������
            while (!operationStack.empty() && priority(ch) <= priority(operationStack.top())) { //���� ��������� �������� ����, ��� � �����, �� �������� �� ����� ��������� � ������
                putTopFromStackToNotation();
            }
            operationStack.push(ch);
            if (ch == 's' || ch == 'c') { i += 2; } // ������� 2 �������� ��� sin, cos
        }
    }
    while (!operationStack.empty()) { //������� ������� ����� � ����� �������� ������ ����� ���������� ������ commonExpression
        putTopFromStackToNotation();
    }
    return PolishNotation;
}

float calculatePolishNotation(string PolishNotation) {
    stack <float> operandStack;
    char operations2[] = {'+', '-', '*', '/', '^'};
    float operand1 = 0;
    float operand2 = 0;

    for (int i = 0; i < PolishNotation.length(); i++) {
        char ch = PolishNotation[i];
        if (isdigit(ch)) {  //��� �����
            operandStack.push(strtof(&ch, NULL));
        }
        else {  //��� ��������
            if (count(begin(operations2), end(operations2), ch)) { // �������� � 2 ����������
                operand2 = operandStack.top();
                operandStack.pop();
                operand1 = operandStack.top();
                operandStack.pop();
            }
            else { // �������� � 1 ���������
                operand1 = operandStack.top();
                operandStack.pop();
            }

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
                operandStack.push(powf(operand1, operand2));
                break;
            case '~':
                operandStack.push(-1 * operand1);
                break;
            case 's':
                operandStack.push(sinf(operand1 * pi / 180));
                i += 2; // ������� �������� i, n
                break;
            case 'c':
                operandStack.push(cosf(operand1 * pi / 180));
                i += 2;
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
    //string commonExpression = "5+(3*(-5))/(sin(5)^cos(5))"
    cout << "������� ������ � ����������� �����: ";
    string commonExpression;
    cin >> commonExpression;
    string PolishNotation = toPolishNotation(commonExpression);
    cout << "���� ������ � �������� �������: " << PolishNotation << endl;
    float result = calculatePolishNotation(PolishNotation);
    cout << "��������� ����������: " << result << endl;
    return 0;
}