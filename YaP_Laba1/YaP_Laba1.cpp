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
        return -1;  //Для sin, cos и скобок
}

string toPolishNotation(string commonExpression) {
    stack <char> operationStack;
    string PolishNotation;
    for (char ch : commonExpression) {
        if (isalnum(ch)) {  //Операнд (цифра или буква)
            PolishNotation += ch;
        }
        else if (ch == '(') {
            operationStack.push(ch);
        }
        else if (ch == ')') {
            while (!operationStack.empty() && operationStack.top() != '(') { //Все операции до ( переносятся в конец польской записи
                PolishNotation += operationStack.top();
                operationStack.pop();
            }
            operationStack.pop();  //Удаление (
        }
        else {  //Операция
            while (!operationStack.empty() && priority(ch) <= priority(operationStack.top())) { //Если приоритет операции выше, чем в стеке, то операции из стека заносятся в запись
                PolishNotation += operationStack.top();
                operationStack.pop();
            }
            operationStack.push(ch);
        }
    }
    while (!operationStack.empty()) { //Перевод остатка стека в конец польской записи после завершения чтения commonExpression
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
        if (isdigit(ch)) {  //Для чисел
            number += ch;
        }
        else {  //Для операций
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
                cerr << "Такой операции нету: '" << ch << "'" << endl;
                return 0;
            }
        }
    }
    if (!operandStack.empty()) {
        return operandStack.top();
    }
    else {
        cerr << "Стек оказался пустым" << endl;
        return 0;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    //string commonExpression = "5+(3*(-5))/(sin(50)^cos(5))";
    string commonExpression = "2+2*2^(-4)";
    string PolishNotation = toPolishNotation(commonExpression);
    int result = calculatePolishNotation(PolishNotation);
    cout << "Введите запись в стандартной форме: " << commonExpression << endl;
    cout << "Ваша запись в польской нотации: " << PolishNotation << endl;
    cout << "Результат вычислений: " << result << endl;
    return 0;
}